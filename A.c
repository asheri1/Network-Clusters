#include "spmat.h"
#include <stdlib.h>
#include "A.h"
#include "checks.h"


/* Releases the allocated memory for the  struct A */
void free_adj(struct _A *adj){
    free(adj->degrees);
    adj->matrix->free(adj->matrix);
}

/* A data structure that represented the graph for which we would like to find a division *
* in the program, contains the number of nodes in the graph, an array of nodes degree *
* and a sparse matrix that represents the adjacency matrix of the graph */
A* A_allocate(int V, int M, int* degrees, spmat* matrix){
    A* adj;
    adj = malloc(sizeof(A));
    check_allocation_A(&adj);

    adj->V = V;
    adj->M = M;
    adj->degrees = degrees;
    adj->free = free_adj;
    adj->matrix = matrix;

    return adj;
}

