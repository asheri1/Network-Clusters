
/*
this module that defines the modularity matrix B_hat,
and has functions that computes it's leading eigenpair
*/


#include "spmat.h"
#include "A.h"
#include <stdio.h>
#ifndef CLUSTER_B_HAT_H
#define CLUSTER_B_HAT_H

typedef struct _B_hat{

    /* Adjacency matrix of the original graph */
    A* Adj;

    /* A group of vertices */
    int* g;

    /* number of vertices in group g */
    int n_g;

    /* leading eigen value */
    double eigen_value;

    /* 1-norm */
    double norm_1;

    /* f^g[i] is the sum of the i'th row in B[g]_hat */
    double* f;

    /* leading eigen vector */
    double* eigen_vector;

    /* Frees all resources used by A */
    void    (*free)(struct _B_hat *mod_matrix);

}B_hat;

/* Allocates a new struct which is a representation of the modularity *
 * matrix of a given group of vertices */
B_hat* B_hat_allocate(A* Adj, int* g, int n_g);

/* Calculates the leading eigen vector and value of the modularity matrix */
void calc_leading_eigenpair(B_hat *mod_matrix);

/* Calculates B[g]_hat from the representation of B_hat and multiply it with a column vector v - (B[g]_hat)*v */
void calc_B_g_hat_mult_vec(B_hat* mod_matrix,double* b, double* result);

void calc_Ab(B_hat* mod_matrix,double* b, double* result);

void calc_Db(B_hat* mod_matrix,double* b, double* result);

/* Calculates f_g (included matrix shifting inside of it) */
void calc_f(B_hat* mod_matrix);

/* Frees all resources used by B*/
void free_B(B_hat *mod_matrix);



#endif
