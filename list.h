/* 
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#ifndef LIST_H
#define LIST_H  


#define MAX_LISTS   10
#define MAX_NODES   100


/* Opaque list type */
typedef struct LIST 
{
    int head, tail, curr, count;   /* indices into node pool, or INVALID_IDX */
    int before;             /* cursor is before first */
    int after;              /* cursor is after last */
    int link;               /* used only when on the free-list of LISTs */
} LIST;

typedef struct Node 
{
    void  *item;   /* non-predefined data type item */
    int    next , prev;
    int    link;   /* when FREE: next free-node index; when IN-USE: unused */
} NODE;


extern int initialized, free_list_head, free_node_head;
extern LIST pool_lists[MAX_LISTS];
extern NODE pool_nodes[MAX_NODES];

/*  Actual function provided by the user/test program, these are just pointers
 to those functions for the implementation
this is a sort of wildcard */
typedef void (*listItemFreeFn)(void *item);
typedef int  (*listItemComparatorFn)(void *item, void *comparisonArg);

/* Creation & query */
LIST *ListCreate(void);
int   ListCount(LIST *list);
int  nodeAllocator();

/* Navigation (also sets/uses the "current" item) */
void *ListFirst(LIST *list);
void *ListLast(LIST *list);
void *ListNext(LIST *list);
void *ListPrev(LIST *list);
void *ListCurr(LIST *list);

/* Insertion */
int   ListAdd(LIST *list, void *item);       /* after current */
int   ListInsert(LIST *list, void *item);    /* before current */
int   ListAppend(LIST *list, void *item);    /* end */
int   ListPrepend(LIST *list, void *item);   /* front */

/* Removal / concatenation / destroy */
void *ListRemove(LIST *list);                /* remove current, return it */
void  ListConcat(LIST *list1, LIST *list2);  /* list2 consumed */
void  ListFree(LIST *list, listItemFreeFn itemFree);
void *ListTrim(LIST *list);                  /* remove & return last */

/* Search (from current node to last node ) */
void *ListSearch(LIST *list,
                                    listItemComparatorFn comparator,
                                    void *comparisonArg);

#endif