#include "B_hat.h"
#include "A.h"
#include <stdlib.h>
#include <stdio.h>
#include "linked_list_m.h"
#include <float.h>
#include "checks.h"
#include "algorithms.h"



/* algo3 takes sparse matrix adj from type A, and inserting to linkedlist O the groups by best modularity divide.*/

void algorithm3(A* adj, linkedlist** O){
    linkedlist* P; linkedlist* group1; linkedlist* group2;  linkedlist* to_move;
    int* g;
    double* s_vec;
    int n_g, i;
    B_hat* Bg_hat;

    g = calloc(adj->V,sizeof(int));   /*allocating g*/
    check_allocation_int(&g);


    for (i = 0; i < adj->V ; ++i) {     /*initializing group to all vertices*/
        g[i] = i;
    }

    P = linkedlist_allocate();   	/*allocating P*/
    check_allocation_ll(&P);
    create_list(&P, &g, adj->V);	/*putting values in list*/


    while(P != NULL && P->length!=0){		/*checking if P is empty or has group of initialization*/

        if(P->length==1){	/*special case*/
            to_move = remove_first_group(&P);
            free(P);
            P = linkedlist_allocate();
            check_allocation_ll(&P);
        }
        else{	/*regular case*/
            to_move = remove_first_group(&P);
        }

        g = to_move->group;          /* marking groups's value*/
        n_g = to_move->group_size;

        Bg_hat = B_hat_allocate(adj, g, n_g);     /*allocating B_hat*/
        check_allocation_B(&Bg_hat);
        calc_leading_eigenpair(Bg_hat);			/*calculating of eigenpair of B_hat, storing memory in B_hat struct*/

        s_vec = calloc(adj->V, sizeof(double));		/*allocating s_vec*/
        check_allocation_double(&s_vec);

        algorithm2(Bg_hat, g, n_g, &s_vec);		/*calling algo 2 for first dividing to 2 groups in s_vec*/

        group1 = linkedlist_allocate();   /*groups 1 & 2 */
        check_allocation_ll(&group1);
        group2 = linkedlist_allocate();
        check_allocation_ll(&group2);

        /* s_vec represents modularity groups1&2 division right now */


        alogrithm4(Bg_hat, &s_vec);						/*calling dividing modularity maximization function, algo4*/

        make_division(Bg_hat, &group1, &group2, &s_vec);  /* makes groups1&2 division from s_vec, after changing s_vec in algo4*/

        Bg_hat->free(Bg_hat);		 /*free B_hat from memory*/

        free(s_vec);				/*free s_vec*/

        if(group1->group_size==0 || group2->group_size==0){		/*speacial case, all s_vec are zero's*/
        	add_group_as_head(O, &to_move);
        }

        else{
            free(g);
            free(to_move);	/*free the taken node from memory*/

            if(group1->group_size == 1){
                add_group_as_head(O, &group1);	/*add the group1 linkedlist node to be the head of O linkedlist*/
            }
            else{
                add_group_as_head(&P, &group1);   /*add the group1 linkedlist node to be the head of P*/
            }

            if(group2->group_size == 1){
                add_group_as_head(O, &group2);
            }
            else{
                add_group_as_head(&P, &group2);
            }
        }
	}
    delete_ll(P);
}

void algorithm2(B_hat* Bg_hat, int* g, int n_g, double** s_vec){		/*algo2 making s_vec to modulizing for groups later*/
    int i;
    double* result;
    double sum;
    int V;

    V = Bg_hat->Adj->V;

    if(Bg_hat->eigen_value <= 0) {		/*checking if group is at max modulation state already*/
        indivisible(s_vec, g, n_g);
    }

    calc_s(Bg_hat, s_vec, g, n_g);		/*calculating s* itself*/


    result = calloc(V, sizeof(double));
    check_allocation_double(&result);

    calc_B_g_hat_mult_vec(Bg_hat, *s_vec, result);	/*putting into result the mult of B_hat*s_vec*/

    for (i = 0; i < V; ++i) {
        sum+= result[i]* (*s_vec)[i];		/*calculating sum*/
    }

    if(sum<=0){			/*checking sum*/
        indivisible(s_vec, g, n_g);
    }

    free(result);

}




void make_division(B_hat* Bg_hat, linkedlist** group1, linkedlist** group2, double** s_vec){   /*using s for dividing to two groups*/
    int i, k;
    int counter1, counter2;
    int* g1; int* g2;

    g1 = calloc(Bg_hat->n_g, sizeof(int));
    check_allocation_int(&g1);

    g2 = calloc(Bg_hat->n_g, sizeof(int));
    check_allocation_int(&g2);

    counter1 = 0;
    counter2 = 0;

    for (i = 0; i < Bg_hat->n_g; ++i) {

        k = (Bg_hat->g)[i];       /* k is B_hat at the i'th palce*/

        if( (*s_vec)[k]==1.0){
            *(g1+counter1) = k;  /*g1[counter1] = k*/
            counter1 +=1;
        }

        if( (*s_vec)[k] == -1.0){
            *(g2+counter2) = k;	/*g2[counter2] = k*/
            counter2 +=1;
        }
    }

    create_list(group1, &g1, counter1);		/*putting values of array g1, size1 at group1's linkedlist*/
    create_list(group2, &g2, counter2);		/*putting values of array g2, size1 at group2's linkedlist*/

}


void indivisible(double** s_vec, int* g, int n_g){		/*if there isnt modulation maximization, make all s vec ones*/
    int i, vertice_index;
    for (i = 0; i < n_g; ++i) {
        vertice_index = g[i];
        (*s_vec)[vertice_index] = 1;
    }
}


void calc_s(B_hat* Bg_hat, double** s, int* g, int n_g){	/*calc s_vec using run on Bhat's eigen vector*/
	int i, j;
    double* u;

    u = Bg_hat->eigen_vector;

    for (i = 0; i < n_g; ++i) {

        j = g[i];
        /*index_to_check*/

        if(u[j]>0) {
            (*s)[j] = 1;
        }

        if(u[j] < 0){

            (*s)[j] = -1;
        }

    }

}

void alogrithm4(B_hat* Bhat, double** s_vec){     /*algo4 changing s_vec for maximazition of modularity groups divide */
    int i, j, k, m, j_tag, i_tag, l;
    double delta_q, D_ii, curr_max,A_g_ij, D_g_ij, sum_of_rows;
    int* unmoved;
    int* indices;
    double* improve;
    double* score;
    double* s;
    const double lowest_double = -DBL_MAX;

    s = *s_vec;
    do {
        /* Trying to find an improvement of the partition defined by s */

        improve = calloc(Bhat->n_g, sizeof(double));
        check_allocation_double(&improve);

        indices = calloc(Bhat->n_g, sizeof(int));
        check_allocation_int(&indices);

        unmoved = calloc(Bhat->n_g, sizeof(int));
        check_allocation_int(&unmoved);

        /* Initialize unmoved vertices array */
        for (i = 0; i < Bhat->n_g; ++i) {
            unmoved[i] = i;
        }

        j_tag = 0;
        for (i = 0; i < Bhat->n_g; ++i) {

            score = calloc(Bhat->n_g, sizeof(double));
            check_allocation_double(&score);

            curr_max = lowest_double;
            /* Computing delta_Q for the move of each vertex */
            for (j = 0; j < Bhat->n_g; ++j) {

                if (-1 == unmoved[j]){continue;}

                k = (Bhat->g)[j];

                s[k] = -s[k];

                sum_of_rows = 0;
                for (m = 0; m < Bhat->n_g; ++m) {
                    A_g_ij = (double)(Bhat->Adj->matrix->get_cell(Bhat->Adj->matrix, k, (Bhat->g)[m]));
                    D_g_ij = (double)(((((Bhat->Adj->degrees)[k])*((Bhat->Adj->degrees)[(Bhat->g)[m]]))/(double)(Bhat->Adj->M)));

                    sum_of_rows += (A_g_ij - D_g_ij)*s[(Bhat->g)[m]];
                }

                sum_of_rows = (sum_of_rows)*4*s[k];
                D_ii = 4*((((Bhat->Adj->degrees)[k])*((Bhat->Adj->degrees)[k]))/(double)(Bhat->Adj->M));
                score[j] = sum_of_rows + 4*(D_ii);

                s[k] = -s[k];

                if (score[j] >= curr_max) {
                    curr_max = score[j];
                    j_tag = j;
                }
            }

            s[(Bhat->g)[j_tag]] = -s[(Bhat->g)[j_tag]];
            indices[i] = j_tag;

            if (0 == i){
                improve[i] = score[j_tag];
            }
            else{
                improve[i] = improve[i-1] + score[j_tag];
            }

            unmoved[j_tag] = -1;
            free(score);
        }

        /* Find the maximum improvement of s and update s accordingly */
        curr_max = lowest_double;
        i_tag = 0;
        for (i = 0; i < Bhat->n_g; ++i) {
            if (improve[i] >= curr_max){
                curr_max = improve[i];
                i_tag = i;
            }
        }

        for (i = Bhat->n_g -1 ; i >= i_tag + 1 ; --i) {
            l = indices[i];
            s[(Bhat->g)[l]] = -s[(Bhat->g)[l]];
        }

        if (i_tag == ((Bhat->n_g) - 1)){

            delta_q = 0.0;
        }
        else
        {
            delta_q = improve[i_tag];
        }

        free(improve);
        free(indices);
        free(unmoved);

    }
    while
            (delta_q > 0.00001);
}
