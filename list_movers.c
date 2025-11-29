/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <list.h>

/* returns the number of items in the list */
int ListCount(LIST *list)
{
    printf("   Reached function: ListCount - Getting list count\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListCount\n");
        return -1;
    }
    return list->count;
}

/* Search (from current node to last node ) */
void *ListSearch(LIST *list, listItemComparatorFn comparator,\
     void *comparisonArg)
{
    void *item;
    printf("   Reached function: ListSearch - Searching for item\n");
    if (list == NULL || comparator == NULL) 
    {
        printf("   Error: NULL parameter passed to ListSearch\n");
        return NULL;
    }
    if (list->before == 1 && list->count > 0) 
    {
        list->curr = list->head;
        list->before = 0;
        list->after = 0;
    }
    item = ListCurr(list);
    while (item != NULL) 
    {
        if (comparator(item, comparisonArg) == 1) 
        {
            printf("   ListSearch: Item found\n");
            return item;
        }
        item = ListNext(list);
    }
    printf("      ListSearch: Search complete : item not found\n");
    return NULL;
}                

/* returns a pointer to the first item in list and makes it the current one */
void *ListFirst(LIST *list)
{
    printf("   Reached function: ListFirst - Moving to first item\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListFirst\n");
        return NULL;
    }
    if (list->count == 0) 
    {
        return NULL;
    }
    list->curr = list->head;
    list->before = 0;
    list->after = 0;
    printf("      ListFirst: At first item\n");
    return pool_nodes[list->curr].item;
}

/* returns a pointer to the last item in list and makes it the current one */
void *ListLast(LIST *list)
{
    printf("   Reached function: ListLast - Moving to last item\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListLast\n");
        return NULL;
    }
    if (list->count == 0) 
    {
        return NULL;
    }
    list->curr = list->tail;
    list->before = 0;
    list->after = 0;
    printf("      ListLast: At last item\n");
    return pool_nodes[list->curr].item;
}

/* advances the list's current node by one, 
and returns a pointer to the new current item */
void *ListNext(LIST *list)
{
    printf("   Reached function: ListNext - Moving to next item\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListNext\n");
        return NULL;
    }
    printf("      ListNext: Moved to next item\n");
    if (list->after == 1) 
    {
        return NULL;
    }
    list->curr = pool_nodes[list->curr].next;
    if (list->curr == -1) 
    {
        list->after = 1;
        list->before = 0;
        return NULL;
    }
    return pool_nodes[list->curr].item;
}

/* moves the list's current node back by one, 
and returns a pointer to the new current item */
void *ListPrev(LIST *list)
{
    printf("   Reached function: ListPrev - Moving to previous item\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListPrev\n");
        return NULL;
    }
    printf("      ListPrev: Moved to previous item\n");
    if (list->before == 1) 
    {
        return NULL;
    }
    list->curr = pool_nodes[list->curr].prev;
    if (list->curr == -1) 
    {
        list->before = 1;
        list->after = 0;
        return NULL;
    }
    return pool_nodes[list->curr].item;
}

/* returns a pointer to the current item in the list */
void *ListCurr(LIST *list)
{
    printf("   Reached function: ListCurr - Getting current item\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListCurr\n");
        return NULL;
    }
    printf("      ListCurr: Returning current item\n");
    return list->curr != -1 ? pool_nodes[list->curr].item : NULL;
}
