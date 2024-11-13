
/* defines how we save the adjustments sparse matrix in memory as a module.*/

#include "spmat.h"
#include <stdio.h>
#ifndef CLUSTER_A_H
#define CLUSTER_A_H

typedef struct _A{

    /* Number of vertices in the graph */
    int V;

    /* Total number of degrees in the whole graph */
    int M;

    /* Degrees array of the graph */
    int* degrees;

    /* Adjacency matrix of A as a sparse matrix */
    spmat* matrix;

    /* Frees all resources used by A */
    void	(*free)(struct _A *adj);

}A;

/* Allocates a new struct which is a representation of an adjacency matrix of a graph */
A* A_allocate(int V, int M, int* degrees, spmat* matrix);

#endif
