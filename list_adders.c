/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <list.h>

int nodeAllocator()
{
    int i;
    if (free_node_head == -1)
    {
        printf("   Error: No more nodes can be allocated.\
 Max limit reached.\n");
        return -1;
    }
    i = free_node_head;
    free_node_head = pool_nodes[i].link; /* update free node head */
    pool_nodes[i].link = -1; /* remove from free list */
    /* We write the below lines just to make sure 
    there is no accidental linking */
    pool_nodes[i].next = -1;
    pool_nodes[i].prev = -1;
    return i;
}

/* after current node */
int ListAdd(LIST *list, void *item)
{
    int newNodeIdx;
    printf("   Reached function: ListAdd - Adding item after current position\n");
    if (list == NULL || item == NULL)
    {
        printf("   Error: NULL parameter passed to ListAdd\n");
        return -1;
    }
    if (list->count == 0 || list->after == 1 || list->curr == -1)
    {
        /* if list is empty or after last, add is same as append */
        return ListAppend(list, item);
    }
    else if (list->before == 1)
    {
        /* if before first, add is same as prepend */
        return ListPrepend(list, item);
    }
    newNodeIdx = nodeAllocator();
    if (newNodeIdx == -1)
    {
        return -1; /* allocation failed */
    }
    pool_nodes[newNodeIdx].prev = list->curr;
    pool_nodes[newNodeIdx].item = item;
    pool_nodes[newNodeIdx].next = pool_nodes[list->curr].next;
    if (pool_nodes[list->curr].next != -1)
    {
        pool_nodes[pool_nodes[list->curr].next].prev = newNodeIdx;
    }
    pool_nodes[list->curr].next = newNodeIdx;
    list->count++; 
    printf("      ListAdd: Successfully added item\n");
    return 0;
}

/* before current node */
int ListInsert(LIST *list, void *item)
{
    printf("   Reached function: ListInsert - \
Inserting item before current position\n");
    if (list == NULL || item == NULL) 
    {
        printf("   Error: NULL parameter passed to ListInsert\n");
        return -1;
    }
    if (list->count == 0)
    {
        /* if list is empty, add is same as insert */
        return ListAppend(list, item);
    }
    else if (list->before == 1)
    {
        /* if before first, insert is same as prepend */
        return ListPrepend(list, item);
    }
    else if (list->after == 1)
    {
        /* if after last, insert is same as append */
        return ListAppend(list, item);
    }
    else 
    {
        int newNodeIdx = nodeAllocator();
        if (newNodeIdx == -1) 
        {
            return -1; /* allocation failed */
        }
        pool_nodes[newNodeIdx].item = item;
        pool_nodes[newNodeIdx].next = list->curr;
        pool_nodes[newNodeIdx].prev = pool_nodes[list->curr].prev;
        if (pool_nodes[list->curr].prev != -1) 
        {
            pool_nodes[pool_nodes[list->curr].prev].next = newNodeIdx;
        }
        pool_nodes[list->curr].prev = newNodeIdx;
        if (list->head == list->curr) 
        {
            /* update head if curr and head were same */
            list->head = newNodeIdx;
        }
        /* current now points to the newly inserted node */
        list->curr = newNodeIdx; 
        list->count++;
    }
    printf("      ListInsert: Successfully inserted item %d\n",\
 *(int *)pool_nodes[list->curr].item); /* remove this comment and the print */
    return 0;
}

/* similar to python lists, append adds nodes to the end of the chain */
int ListAppend(LIST *list, void *item)
{
    int newNodeIdx;
    printf("   Reached function: ListAppend - Adding item to end of list\n");
    if (list == NULL || item == NULL) 
    {
        printf("   Error: NULL parameter passed to ListAppend\n");
        return -1;
    }
    newNodeIdx = nodeAllocator();
    if (newNodeIdx == -1) 
    {
        return -1; /* allocation failed */
    }
    pool_nodes[newNodeIdx].item = item;
    if (list->count == 0) 
    {
        /* List is empty */
        list->head = newNodeIdx;
        list->tail = newNodeIdx;
        list->curr = newNodeIdx;
        list->before = 0;
        list->after = 0;
    } 

    pool_nodes[newNodeIdx].prev = list->tail;
    pool_nodes[list->tail].next = newNodeIdx;
    list->tail = newNodeIdx;
    list->count++;
    list->before = 0;
    list->after = 0;
    printf("      ListAppend: Successfully appended item %d\n",\
 *(int *)pool_nodes[list->tail].item); /* remove this comment and the print */
    return 0;
}

/* prepend adds nodes to the front of the chain */
int ListPrepend(LIST *list, void *item)
{
    int newNodeIdx;
    printf("   Reached function: ListPrepend - Adding item to front of list\n");
    if (list == NULL || item == NULL) 
    {
        printf("   Error: NULL parameter passed to ListPrepend\n");
        return -1;
    }
    newNodeIdx = nodeAllocator();
    if (newNodeIdx == -1) 
    {
        return -1; /* allocation failed */
    }
    pool_nodes[newNodeIdx].item = item;
    if (list->count == 0) 
    {
        /* List is empty */
        list->head = newNodeIdx;
        list->tail = newNodeIdx;
        list->curr = newNodeIdx;
        list->before = 0;
        list->after = 0;
    } 
    pool_nodes[newNodeIdx].item = item;
    pool_nodes[newNodeIdx].next = list->head;
    pool_nodes[newNodeIdx].prev = -1;
    pool_nodes[list->head].prev = newNodeIdx;
    list->head = newNodeIdx; /* update head if curr and head were same */
    list->count++;
    printf("      ListPrepend: Successfully prepended item %d\n",\
 *(int *)pool_nodes[list->curr].item); /* remove this comment and the print */
    return 0;
}
