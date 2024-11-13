#include "B_hat.h"
#include "A.h"
#include "spmat.h"
#include "stdlib.h"
#include <math.h>
#include "checks.h"

void generate_b(B_hat* mod_matrix,double** b);
void calc_fb(B_hat* mod_matrix,double* b, double* result);
int check_if_smaller_than_epsilon(double *prev_b, double *next_b, double epsilon, int size);
void calc_magnitude_and_divide(double *vector, int size);
void power_iteration(B_hat* mod_matrix, double* result, double* prev_b, double* next_b, double epsilon);
void calc_eigen_value(B_hat* mod_matrix, double* eigen_vector, double* eigen_value);


/* A data structure given a graph A and its set of nodes g represents its modularity matrix B[g]_hat *
 * allows us to calculate the leading pair of the matrix */
B_hat* B_hat_allocate(A* adj, int* g, int n_g){
    B_hat* B;
    double* eigen_vector;
    double* f;

    B = malloc(sizeof(B_hat));
    check_allocation_B(&B);

    eigen_vector = calloc(((*adj).V), sizeof(double));
    check_allocation_double(&eigen_vector);

    f = calloc(((*adj).V), sizeof(double));
    check_allocation_double(&f);

    B->free = free_B;
    B->Adj = adj;
    B->g = g;
    B->n_g = n_g;
    B->eigen_vector = eigen_vector;
    B->f = f;

    calc_f(B);

    return B;
}

/* Releases the allocated memory for the  struct A */
void free_B(struct _B_hat* mod_matrix){

    free(mod_matrix->eigen_vector);

    free(mod_matrix->f);
}


/* Given a matrix B[g]_hat the function will calculate the leading eigen pair of it */
void calc_leading_eigenpair(struct _B_hat *mod_matrix){
    int V;
    double* b;
    double* b_temp;
    const double epsilon = 0.00001;

    V = (*((A*)(mod_matrix->Adj))).V;   /* load the amount of vertices in the graph from struct A */
    b_temp = calloc(V, sizeof(double)); /* allocating temp vector - to be used in the power iteration */
    check_allocation_double(&b_temp);

    /* Initialize a new vector size V according to the nodes in group g to execute power iteration *
     * the i'th element in b is 0 iff the i'th node is not in group g */
    b = calloc(V, sizeof(double));
    check_allocation_double(&b);


    generate_b(mod_matrix, &b);

    /* Execution of power iteration over B[g]_hat  with the vector we have initialized *
     * in order to find the leading eigen vector of the matrix */
    power_iteration(mod_matrix, mod_matrix->eigen_vector, b_temp, b ,epsilon);

    /* Calculation of B[g]_hat's leading eigen value */
    calc_eigen_value(mod_matrix, mod_matrix->eigen_vector, &(mod_matrix->eigen_value));

    /* Free allocated memory */
    free(b_temp);

    free(b);
}
/* The function randomly generates a vector */
void generate_b(B_hat* mod_matrix,double** b){
    int i, counter, n_g;
    int* g;

    g = mod_matrix->g;
    n_g = mod_matrix->n_g;
    counter = 0;


    /* If the i'th node is in group g b[i] = random integer, else b[i] = 0 */
    for (i = 0; i < n_g; ++i) {
        if (i == *g){
            (*b)[i] = rand();
            g++;
        }
    }
    g -= counter;
}

void power_iteration(B_hat* mod_matrix, double* result, double* prev_b, double* next_b, double epsilon) {
    int V;
    int val = 0;

    V = ((A*)(mod_matrix->Adj))->V;    /* load number of vertices in the graph from struct A */


    /* At first prev_b is an empty vector and next_b is the random vector *
    * We will continue the iteration as long as between the two vectors greater than epsilon */
    while (val == 0) {

        calc_B_g_hat_mult_vec(mod_matrix, next_b, result);  /* Multiply the vector by the matrix */

        calc_magnitude_and_divide(result, V);  /* Normalize the result */

        val = check_if_smaller_than_epsilon(next_b, result, epsilon, V);

        /* After the multiplication, prev_b is now next_b and next_b is promoted to be result of the multiplication */
        prev_b = next_b;

        next_b = result;

        result = prev_b;

    }
}

void calc_B_g_hat_mult_vec(B_hat* mod_matrix,double* b, double* result){
    /* To calculate the product of the matrix and the vector we break down the matrix into factors *
     * in the following way ---> (B[g]_hat)*b = (A*b) - (D*b) - (f*b) *
     * where A is the adjacency matrix, D is the matrix of (k_i)*(k_j)/M and f_i is the sum of the *
     * i'th row in B[g]_hat included matrix shifting inside of it */
    calc_Ab(mod_matrix, b, result);
    calc_Db(mod_matrix, b, result);
    calc_fb(mod_matrix, b, result);
}

void calc_Ab(B_hat* mod_matrix,double* b, double* result){
    spmat* sm;
    sm = ((A*)(mod_matrix->Adj))->matrix;
    sm->mult(sm, b, result);
}

void calc_Db(B_hat* mod_matrix,double* b, double* result){
    int i, M;
    double sum;
    int* deg;

    M = ((A*)(mod_matrix->Adj))->M;
    deg = ((A*)(mod_matrix->Adj))->degrees;
    sum = 0;

    /* multiply the vectors (k1 k2 ... kn) * (b1 b2 ... bn) */
    for (i = 0; i < mod_matrix->n_g; ++i) {
        sum += deg[(mod_matrix->g)[i]]*b[(mod_matrix->g)[i]];
    }

    /* multiply the result from above with 1/M * (k1 k2 ... kn) *
     * to obtain (B[g]_hat)*b */
    for (i = 0; i < mod_matrix->n_g; ++i) {
        (result)[(mod_matrix->g)[i]] -= sum*((double)(1/(double)M))*(deg[(mod_matrix->g)[i]]);
    }
}

void calc_fb(B_hat* mod_matrix,double* b, double* result){
    int i;

    for (i = 0; i < mod_matrix->n_g; ++i) {
        (result)[(mod_matrix->g)[i]] -= ((mod_matrix->f)[i]) * b[(mod_matrix->g)[i]];
    }
}


/* The function calculates and updates the eigen value of the matrix B[g]_hat */
void calc_eigen_value(B_hat* mod_matrix, double* eigen_vector, double* eigen_value){
    double* Abk;
    double denominator, numerator;

    spmat* sm;
    int V, i;

    numerator = 0.0;
    denominator = 0.0;

    V = ((A*)(mod_matrix->Adj))->V;
    Abk = calloc(V, sizeof(double));
    check_allocation_double(&Abk);


    sm = ((A*)(mod_matrix->Adj))->matrix;
    sm->mult(sm, eigen_vector, Abk);

    for (i = 0; i < V; ++i) {
        numerator += eigen_vector[i]*Abk[i];
        denominator += eigen_vector[i]*eigen_vector[i];
    }
    *eigen_value += numerator/denominator;

    free(Abk);
}

/* We will examine whether the difference between the obtained vector *
 * and the previous vector we calculated is smaller than that of epsilon */
int check_if_smaller_than_epsilon(double *prev_b, double *next_b, double epsilon, int size) {
    int i;
    double difference = 0.0;

    for (i = 0; i < size; i++) {
        difference = fabs(next_b[i] - prev_b[i]);
        if (difference >= epsilon) {
            return 0;
        }
    }
    return 1;
}


/* The function receives a pointer to a vector of doubles and normalizes it */
void calc_magnitude_and_divide(double *vector, int size) {
    double sum = 0;
    double norm = 0;
    int i;
    for (i = 0; i < size; i++) {
        sum += (vector[i]) * (vector[i]);
    }
    norm = sqrt(sum);
    for (i = 0; i < size; i++) {
        vector[i] = (vector[i]) / norm;
    }
}

void calc_f(B_hat* mod_matrix){
    int i, n_g, j, k_i, k_j, M;
    double a_ij, D_ij, curr_max_col, curr_col;

    n_g = mod_matrix->n_g;
    M = (*((A*)(mod_matrix->Adj))).M;

    curr_max_col = 0;
    curr_col = 0;

    for (i = 0; i < n_g; ++i) {
        for (j = 0; j < n_g; ++j) {
            a_ij = get_cell_array((mod_matrix->Adj)->matrix, (mod_matrix->g)[i], (mod_matrix->g)[j]);
            k_i = ((mod_matrix->Adj)->degrees)[(mod_matrix->g)[i]];
            k_j = ((mod_matrix->Adj)->degrees)[(mod_matrix->g)[j]];
            D_ij = (((double)(k_i*k_j))/((double)M));

            (mod_matrix->f)[i] += a_ij - D_ij;
            curr_col += fabs(a_ij - D_ij);
        }

        /* We will want to take advantage of the transition on B[g]_hat *
        * line to calculate the on the way the matrix shifting (because the matrix is symetric  */
        if (curr_col  > curr_max_col){
            curr_max_col = curr_col;
        }
        curr_col = 0;
    }


    mod_matrix->norm_1 = curr_max_col;

    /* matrix shifting */
    for (i = 0; i < mod_matrix->n_g; ++i) {
        mod_matrix->f[i] -= mod_matrix->norm_1;
    }

}
