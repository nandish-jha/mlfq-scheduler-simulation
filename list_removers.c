/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <list.h>


/* Creates a new empty list and returns pointer to it */
LIST *ListCreate(void)
{
    int i, freeListIdx;
    printf("      Reached function: ListCreate - Creating new empty list\n");
    /* Runs only once to clean the memory pre-reserved by the pool variables */
    if (initialized == 0) 
    {
        for (i = 0; i < MAX_NODES; i++)
        {
            pool_nodes[i].next = -1;
            pool_nodes[i].prev = -1;
            pool_nodes[i].item = NULL;
            pool_nodes[i].link = i + 1;
        }
        pool_nodes[MAX_NODES - 1].link = -1; /* end of free nodes */
        for (i = 0; i < MAX_LISTS; i++)
        {
            pool_lists[i].head = -1;
            pool_lists[i].tail = -1;
            pool_lists[i].curr = -1;
            pool_lists[i].count = 0;
            pool_lists[i].before = 1;
            pool_lists[i].after = 0;
            pool_lists[i].link = i + 1;
        }
        pool_lists[MAX_LISTS - 1].link = -1; /* end of free list */
        initialized = 1;
    }
    /* Allocate a list from the pool */
    freeListIdx = free_list_head;
    if (freeListIdx == -1) 
    {
        printf("   Error: No more lists can be created. Max limit reached.\n");
        return NULL;
    }
    free_list_head = pool_lists[freeListIdx].link; /* update free list head */
    pool_lists[freeListIdx].link = -1;
    printf("      ListCreate: List created successfully\n");
    return &pool_lists[freeListIdx];
}

/*  Return current item and take it out of list */
void *ListRemove(LIST *list)
{
    int prevNode, nextNode, currNode;
    void *item;
    printf("   Reached function: ListRemove - Removing current item\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListRemove\n");
        return NULL;
    }
    if (list->count == 0 || list->curr == -1 || list->before == 1 ||\
         list->after == 1) 
    {
        printf("   Error: ListRemove called on empty list \
or invalid cursor\n");
        return NULL;
    }

    currNode = list->curr;
    item = pool_nodes[currNode].item;
    prevNode = pool_nodes[currNode].prev;
    nextNode = pool_nodes[currNode].next;

    /*Reset the values stored in the removed node*/
    pool_nodes[currNode].item = NULL;
    pool_nodes[currNode].next = -1;
    pool_nodes[currNode].prev = -1;
    pool_nodes[currNode].link = free_node_head; /* add to free list */
    free_node_head = currNode; /* update free node head */

    /* Update node pointers */
    if (prevNode != -1) 
    {
        pool_nodes[prevNode].next = nextNode;
    }
    else 
    {
        list->head = nextNode;
    }
    if (nextNode != -1) 
    {
        pool_nodes[nextNode].prev = prevNode;
    }
    else 
    {
        list->tail = prevNode;
    }
    list->count--;
    if (list->count == 0) 
    {
        /* list is now empty */
        list->curr = -1;
        list->before = 1;
        list->after = 0;
    }
    else if (nextNode != -1) 
    {
        /* move curr to next node */
        list->curr = nextNode;
        list->before = 0;
        list->after = 0;
    }
    else 
    {
        /* move curr to before last node */
        list->curr = prevNode;
        list->before = 0;
        list->after = 0;
    }
    printf("      ListRemove: Item removed\n");
    return item;
}

/* adds list2 to the end of list1 */
void ListConcat(LIST *list1, LIST *list2)
{
    printf("   Reached function: ListConcat - Concatenating two lists\n");
    if (list1 == NULL || list2 == NULL) 
    {
        printf("   Error: NULL list passed to ListConcat\n");
        return;
    }
    if (list2->count == 0) 
    {
        /* nothing to concatenate */
        return;
    }
    pool_nodes[list1->tail].next = list2->head;
    pool_nodes[list2->head].prev = list1->tail;
    list1->tail = list2->tail;
    list1->count = list1->count + list2->count;
    list2->head = -1, list2->tail = -1, list2->curr = -1;
    list2->count = 0, list2->before = 1, list2->after = 0;
    /* Add list2 back to free list */
    list2->link = free_list_head;
    /* pointer arithmetic to get index */
    free_list_head = (int)(list2 - pool_lists); 

    printf("      ListConcat: Lists concatenated (stub implementation)\n");
}

/* delete list. itemFree is a pointer to a routine that frees an item */
void ListFree(LIST *list, listItemFreeFn itemFree)
{
    void *item;
    printf("   Reached function: ListFree - Freeing list and all items\n");
    if (list == NULL || itemFree == NULL) 
    {
        printf("   Error: NULL list or Invalid itemFree passed to ListFree\n");
        return;
    }
    ListFirst(list);
    while (list->count > 0) 
    {
        item = ListCurr(list);
        if (item != NULL) 
        {
            (*itemFree)(item);
        }
        item = ListNext(list);
        list->count--;
    }
    /* Add list back to free list */
    list->head = -1;
    list->tail = -1;
    list->curr = -1;
    list->count = 0;
    list->before = 1;
    list->after = 0;
    list->link = free_list_head;
    /* pointer arithmetic to get index */
    free_list_head = (int)(list - pool_lists);
    printf("      ListFree: List freed successfully \n");
    return;
}

/* Return last item and take it out of list */
void *ListTrim(LIST *list)
{
    int savedCursor;
    void *item;
    printf("   Reached function: ListTrim - Removing and returning last item\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListTrim\n");
        return NULL;
    }
    if (list->count == 0) 
    {
        return NULL;
    }
    /* Move the cursor to the tail, call remove and restore cursor */
    if (list->curr == list->tail) 
    {
        /* if already at tail, just call remove */
        return ListRemove(list);
    }
    savedCursor = list->curr;
    ListLast(list);
    item = ListRemove(list);
    list->curr = savedCursor;
    printf("      ListTrim: Last item trimmed\n");
    return item;
}
