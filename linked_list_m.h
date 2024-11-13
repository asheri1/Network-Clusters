
/*module that should be able to represent a linkedlist of nodes.
each one contains array of vertices, and other fields*/




#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

typedef struct{
    int length;
    int* group;
    int group_size;
    struct linkedlist* next;
}linkedlist;


/* Makes allocation in memory to linkedlist */
linkedlist* linkedlist_allocate();

/* After a new linkedlist alocated - initialize with parameters */
void create_list(linkedlist** list, int** g, int n_g);

/* Makes new_group to be head of list*/
void add_group_as_head(linkedlist** list, linkedlist** new_group);

/* Removes the first group of the list from it, and returns it to USER*/
linkedlist* remove_first_group(linkedlist** list);

/* Deletes all nodes from list */
void delete_ll(linkedlist* head);


#endif /* LINKEDLIST_H_ */
