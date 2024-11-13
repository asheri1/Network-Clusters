
/*module that given an adjastment sparse matrix A,
makes modularity into groups by the following algorithms.

the groups returns as linkedlist of nodes that contains them by the modularity
divide made by algorithms 2,3 and 4.*/

#include "B_hat.h"
#include "linked_list_m.h"
#ifndef COMMUNITY_STRUCTURE_IN_NETWORKS_ALGORITHMS_H
#define COMMUNITY_STRUCTURE_IN_NETWORKS_ALGORITHMS_H

/* algo3 takes sparse matrix adj from type A, and inserting to linkedlist O the groups by best modularity divide.*/
void algorithm3(A* adj, linkedlist** O);

/*algo2 making s_vec to modulizing for groups later*/
void algorithm2(B_hat* Bg_hat, int* g, int n_g, double** s_vec);

/*if there isnt modulation maximization, make all s vec ones*/
void indivisible(double** s_vec, int* g, int n_g);

/*calculating s_vec using run on Bhat's eigen vector*/
void calc_s(B_hat* Bg_hat, double** s, int* g, int n_g);

/*using s for dividing to two groups*/
void make_division(B_hat* Bg_hat, linkedlist** group1, linkedlist** group2, double** s_vec);



/*algo4 changing s_vec for maximazition of modularity groups divide */
void alogrithm4(B_hat* Bhat, double** s);




#endif
