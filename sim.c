/*
Nandish Jha NAJ474 11282001
*/

#include <stdio.h>
#include <os.h>
#include <list.h>

#define SCHEDULER_QUANTUM 25

#define BLOCK_MAXIMUM 50
#define BLOCK_CHANCE 3
#define WORK_MAXIMUM 50

#define PRIORITY_LEVELS 5

/* Global variables for list management */
int initialized = 0;
int free_list_head = 0;
int free_node_head = 0;
LIST pool_lists[MAX_LISTS];
NODE pool_nodes[MAX_NODES];

void panic(const char *msg) {
    fprintf(stderr, "[PANIC] %s\n", msg);
    exit(1);
}

void error(const char *msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
}

enum pstate {NONE, RUNNING, RUNNABLE, BLOCKING};

struct proc {
    PID pid;
    enum pstate state;
    int priority;
};

static int ncpu;

static struct {
    int mutex;
    int running;
    struct proc *procs;
    size_t size;
} ptable;

static PID scheduler_pid;

/* Ready Queues for processes based on priority */
static LIST *prio_0_Queue; /* Ready Queue for processes based on priority */
static LIST *prio_1_Queue; /* Ready Queue for processes based on priority */
static LIST *prio_2_Queue; /* Ready Queue for processes based on priority */
static LIST *prio_3_Queue; /* Ready Queue for processes based on priority */
static LIST *prio_4_Queue; /* Ready Queue for processes based on priority */

/* Comparison function for process pointers */
int proc_ptr_cmp(void *item, void *comparisonArg) {
    return item == comparisonArg;
}

/* Returns the next process to run or NULL if there is nothing runnable */
struct proc *next_proc() {
    struct proc *p = NULL;

    /* Check priority queues from highest (0) to lowest (4) */
    if (ListFirst(prio_0_Queue)) {
        p = ListRemove(prio_0_Queue);
        return p;
    }
    if (ListFirst(prio_1_Queue)) {
        p = ListRemove(prio_1_Queue);
        return p;
    }
    if (ListFirst(prio_2_Queue)) {
        p = ListRemove(prio_2_Queue);
        return p;
    }
    if (ListFirst(prio_3_Queue)) {
        p = ListRemove(prio_3_Queue);
        return p;
    }
    if (ListFirst(prio_4_Queue)) {
        p = ListRemove(prio_4_Queue);
        return p;
    }

    return NULL;
}

/* Scheduler entrypoint */
void scheduler(void *arg) {
    struct proc *p;

    for (;;) {
        if (P(ptable.mutex)) panic("invalid ptable mutex");

        while (ptable.running < ncpu) {
            p = next_proc();
            if (!p) break;
            if (Resume(p->pid)) continue;
            p->state = RUNNING;
            ptable.running += 1;
        }

        if (V(ptable.mutex)) panic("invalid ptable mutex");

        Sleep(SCHEDULER_QUANTUM);
    }
}

/* Sets the process state */
void set_state(enum pstate state) {
    PID pid;
    struct proc *p;

    pid = MyPid();

    if (state == RUNNING && pid != scheduler_pid)
        panic("only the scheduler can set things to running!");

    if (P(ptable.mutex)) panic("invalid ptable mutex");

    for (p = ptable.procs; p < &ptable.procs[ptable.size]; p++) {
        if (p->pid != pid) continue;

        /* Don't do anything because it's not going to change... */
        if (p->state == state) break;

        if (p->state == RUNNING) {
            ptable.running -= 1;
        }

        /* Remove from the queue if leaving RUNNABLE */
        if (p->state == RUNNABLE && state != RUNNABLE) {
            LIST *queue = NULL;
            switch (p->priority) {
                case 0: queue = prio_0_Queue; break;
                case 1: queue = prio_1_Queue; break;
                case 2: queue = prio_2_Queue; break;
                case 3: queue = prio_3_Queue; break;
                case 4: queue = prio_4_Queue; break;
            }
            if (queue) {
                ListFirst(queue);
                if (ListSearch(queue, proc_ptr_cmp, p)) {
                    ListRemove(queue);
                }
            }
        }

        /* Add to the queue if becoming RUNNABLE */
        if (state == RUNNABLE) {
            switch (p->priority) {
                case 0: ListAppend(prio_0_Queue, p); break;
                case 1: ListAppend(prio_1_Queue, p); break;
                case 2: ListAppend(prio_2_Queue, p); break;
                case 3: ListAppend(prio_3_Queue, p); break;
                case 4: ListAppend(prio_4_Queue, p); break;
            }
        }

        p->state = state;
        break;
    }

    if (V(ptable.mutex)) panic("invalid ptable mutex");
}

/* Returns the priority of the calling process */
int get_priority() {
    PID pid;
    struct proc *p;
    int priority = -1;

    pid = MyPid();

    if (P(ptable.mutex)) panic("invalid ptable mutex");

    for (p = ptable.procs; p < &ptable.procs[ptable.size]; p++) {
        if (p->pid != pid) continue;
        priority = p->priority;
        break;
    }

    if (V(ptable.mutex)) panic("invalid ptable mutex");

    return priority;
}

/* Causes the running process to yield to others */
void yield() {
    set_state(RUNNABLE);
    Suspend();
}

/* Simulates a blocking call */
void block(unsigned int time) {
    set_state(BLOCKING);
    Sleep(time);
    yield();
}

/* Process entry point */
void process(void *arg) {
    char *name = (char *) arg;

    set_state(RUNNABLE);
    /* We need to wait for the scheduler to let us start. */
    Suspend();

    for (;;) {
        if (rand() % BLOCK_CHANCE == 0) {
            printf("%s is start block.\n", name);
            block(rand() % BLOCK_MAXIMUM);
            printf("%s is done blocking.\n", name);
        } else {
            printf("%s is running. (priority %d)\n", name, get_priority());
            Sleep(rand() % WORK_MAXIMUM);
            yield();
        }
    }
}

int mainp(int argc, char **argv) {
    PID pid;
    char *name;
    int i;
    struct proc *p;

    srand(0);

    if (argc != 3) {
        printf("usage: %s nproc ncpu\n", argv[0]);
        exit(1);
    }

    ptable.size = atoi(argv[1]);
    if (ptable.size <= 0) {
        printf("nproc must be greater than 0\n");
        exit(1);
    }

    ncpu = atoi(argv[2]);
    if (ncpu <= 0) {
        printf("ncpu must be greater than 0\n");
        exit(1);
    }

    ptable.running = 0;
    ptable.procs = calloc(sizeof(struct proc), ptable.size);
    if (ptable.procs == NULL) {
        panic("Unable to allocate ptable.");
    }

    ptable.mutex = NewSem(1);
    if (ptable.mutex == -1) {
        panic("Unable to create semaphore for ptable.");
    }

    /* Initialize the runnable queues */
    prio_0_Queue = ListCreate();
    prio_1_Queue = ListCreate();
    prio_2_Queue = ListCreate();
    prio_3_Queue = ListCreate();
    prio_4_Queue = ListCreate();
    
    if (!prio_0_Queue || !prio_1_Queue || !prio_2_Queue ||
        !prio_3_Queue || !prio_4_Queue) {
        panic("Unable to create runnable queues.");
    }

    scheduler_pid = Create(scheduler, 4096, "scheduler", NULL, HIGH, USR);
    if (scheduler_pid == PNUL) {
        panic("Unable to create scheduler thread.");
    }

    for (i = 0; i < ptable.size; i++) {
        name = malloc(32);
        if (name == NULL) {
            panic("Unable to allocate memory for name");
        }
        sprintf(name, "process_%d", i);

        pid = Create(process, 4096, name, name, NORM, USR);
        if (pid == PNUL) {
            panic("Unable to create thread for process.");
        }

        p = &ptable.procs[i];
        p->pid = pid;
        p->state = NONE;
        p->priority = i % 5;  /* Assign priorities 0,1,2,3,4 in order */
        printf("Created process_%d with priority %d\n", i, p->priority);
    }

    return 0;
}
