#include "linked_list_m.h"
#include "checks.h"
#include <stdlib.h>
#include <stdio.h>

/* Makes allocation in memory */
linkedlist* linkedlist_allocate(){

    linkedlist* list;
    int* groupy;

    list = malloc(sizeof(linkedlist));
    check_allocation_ll(&list);

    groupy = calloc( 1 , sizeof(int));
    check_allocation_int(&groupy);

    /* Initialization with default parameters*/
    list->length = 0;
    list->group = groupy;        /* Inner group is malloced */
    list->group_size = 0;
    list -> next = NULL;

    return list;
}

/* After a new linkedlist alocated - initialize with parameters */
void create_list(linkedlist** list, int** g, int n_g){

    int* old_g;
    old_g = (*list)->group;

    (*list)->group= (*g);

    free(old_g);

    (*list)->group_size = n_g;
    (*list)->length = 1;
}

/* Makes new_group to be head of list*/
void add_group_as_head(linkedlist** list, linkedlist** new_group){

    (*new_group)->next = (struct linkedlist*)(*list);
    (*new_group)->length = (*list)->length + 1;
    *list = *new_group;
}


/* Removes the first group of the list from it, and returns it to USER*/
linkedlist* remove_first_group(linkedlist** list){
    linkedlist* removed_group;

    removed_group = *list;

    *list = (linkedlist*)((*list)->next);

    removed_group->next = NULL;
    removed_group->length = 1;

    return removed_group;
}


/* Deletes all nodes from list */
void delete_ll(linkedlist* head){
    if(head!=NULL){
        delete_ll((linkedlist*)head->next);

        free(head);
    }
}

