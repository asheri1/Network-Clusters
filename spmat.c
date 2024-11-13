#include "spmat.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


void add_row_ll(struct _spmat *A, const double *row, int i);
void free_ll(struct _spmat *A);
void mult_ll(const struct _spmat *A, const double *v, double *result);

void add_row_array(struct _spmat *A, const double *row, int i);
void free_array(struct _spmat *A);
void mult_array(const struct _spmat *A, const double *v, double *result);

typedef struct{
    double* values;
    int* colind;
    int* rowptr;
}spmat_array;


/* structure that represents a node of linkedList*/
typedef struct{
    double value;
    int column_index;
    struct node *next;
}node;


spmat* spmat_allocate_list(int n){
    int i;
    spmat* mat;
    node** rows_array;

    mat = malloc(sizeof(spmat));

    /* initialize rows_array*/
    rows_array = malloc(n*sizeof(node*));
    for (i = 0; i < n; ++i) {
        rows_array[i] = (node*)malloc(sizeof(node));
    }


    /* set spars matrix parameters */
    mat->n = n;
    mat->private = rows_array;
    mat->add_row = add_row_ll;
    mat->free = free_ll;
    mat->mult = mult_ll;

    return mat;
}


void add_row_ll(struct _spmat *A, const double *row, int i){

    int empty = 1;	 /*flag to check if list is empty*/
    int j;
    double val;
    node* head;
    node* tail;
/*	int m;
	node* head_again;*/


    /* get to rows_array[i] pointer*/
    head = &( *((node**)(A->private))[i] ) ;  /*addressing to address tochen*/


    tail = head;

    for (j = 0; j < A->n ; ++j) {
        if( (val=row[j])!=0 ){
            if(empty==1){  			/*if list is empty, than initialize*/
                assert(head!=NULL);
                head->value = val;
                head->column_index = j;
                head->next = NULL;
                tail = head;
                empty = 0;  		/*list isnt empty anymore*/
            }
            else{
                tail->next = (struct node*)malloc(sizeof(node));
                tail=(node*) tail->next;
                assert(tail!=NULL);
                tail->value = val;
                tail->column_index = j;
                tail->next = NULL;
            }
        }
    }

/*	if(i==(A->n)-1){   only last row
		for (m = 0; m < A->n; ++m) {
			head_again = &( *((node**)(A->private))[m] ) ;
			while(head_again != NULL){
				head_again=(node*)head_again->next;
			}
		}
	}*/
}


/*
void free_ll(struct _spmat *A){
	node* head;
	node* tmp;
	int i;
	for (i = 0; i < A->n; ++i) {
		head = &( *((node**)(A->private))[i] ) ;
		while(head!=NULL){
			tmp = (node*)head->next;
			free(head);
			head = tmp;
		}
	}
}
*/


void delete_list(node* head){
    if(head!=NULL){
        delete_list((node*)head->next);
        free(head);
    }
}


void free_ll(struct _spmat *A){
    node* head;
    int i;

    for (i=0; i < A->n; ++i) {
        head = &( *((node**)(A->private))[i] ) ;
        delete_list(head);
    }
}



void mult_ll(const struct _spmat *A, const double *v, double *result){
    int i;
    int j;
    node* head;

    for (i = 0; i < A->n ; ++i) {
        double sum = 0;
        head = &( *((node**)(A->private))[i] );
        while(head != NULL){    /*multiply row only if list isnt empty*/
            j = head->column_index;     /*mult the ll values only! the rest are zero anyway*/
            sum+=(head->value)*(*(v+j));   /*adds value of array[i][j]*v[j] */
            head=(node*)head->next;
        }
        *(result+i) = sum;
    }
}






/* Allocates a new arrays sparse matrix of size n with nnz non-zero elements */
spmat* spmat_allocate_array(int n, int nnz){
    spmat* spars_matrix;
    spmat_array* spars_matrix_array;

    spars_matrix = malloc(sizeof(spmat));
    spars_matrix_array = malloc(sizeof(spmat_array));

    /* initialize spars_matrix_array assistance arrays*/
    spars_matrix_array->values = (double*)malloc((nnz)*sizeof(double));
    spars_matrix_array->colind = (int*)malloc((nnz)*sizeof(int));
    spars_matrix_array->rowptr = (int*)malloc((n+1)*sizeof(int));
    spars_matrix_array->rowptr[0] = 0;

    /* set spars matrix parameters */
    spars_matrix->private = spars_matrix_array;
    spars_matrix->add_row = add_row_array;
    spars_matrix->free = free_array;
    spars_matrix->mult = mult_array;
    spars_matrix->n = n;
    spars_matrix->get_cell = get_cell_array;

    return spars_matrix;
}

/* Adds row i the matrix. Called before any other call,
 * exactly n times in order (i = 0 to n-1) */
void add_row_array(struct _spmat *A, const double *row, int i){
    int counter, j;

    /* set how many elements i currently have in my matrix*/
    counter = (*(spmat_array*)(A->private)).rowptr[i];
    for (j = 0; j < A->n ; j++) {
        if (row[j] != 0){
            (*(spmat_array*)(A->private)).colind[counter] = j;
            (*(spmat_array*)(A->private)).values[counter] = row[j];
            counter++;
        }

    }
    (*(spmat_array*)(A->private)).rowptr[i+1] = counter;
}

/* Frees all resources used by A */
void free_array(struct _spmat *A){
    free(((spmat_array*)(A->private))->values);
    free(((spmat_array*)(A->private))->colind);
    free(((spmat_array*)(A->private))->rowptr);

}

/* Multiplies matrix A by vector v, into result (result is pre-allocated) */
void mult_array(const struct _spmat *A, const double *v, double *result){
    int i, j, start, end, col;
    double sum, x, y;
    /* first iteration - for every row in the matrix */
    for (i = 0; i < A->n ; i++){

        sum = 0;
        /* set the first index for the second iteration as the i'th element in rowptr */
        start = (*(spmat_array*)(A->private)).rowptr[i];
        /* set the last index for the second iteration as the (i+1)'th element in rowptr */
        end = (*(spmat_array*)(A->private)).rowptr[i+1];

        /* second iteration - for each row, take the correct elements from values[]
         * and mult them with the matching elements in vector v */
        for (j = start ;j < end ; j++) {

            col = (*(spmat_array*)(A->private)).colind[j];

            x = ((*(spmat_array*)(A->private)).values[j]);

            y = (v[col]);

            sum += x*y;

        }
        result[i] = sum;
    }

}



double get_cell_array(const struct _spmat *A, int i, int j){
    int start, end, k;
    double A_ij;
    /* set the first index for the second iteration as the i'th element in rowptr */
    start = (*(spmat_array*)(A->private)).rowptr[i];

    /* set the last index for the second iteration as the (i+1)'th element in rowptr */
    end = (*(spmat_array*)(A->private)).rowptr[i+1];


    if(start==end){     /*all row are zeros*/
        A_ij = 0.0;
        return A_ij;
    }


    else{
        for (k = start ;k < end ; k++) {
            if((*(spmat_array*)(A->private)).colind[k] ==j){      /* colind[k] is col index - j */
                A_ij = (*(spmat_array*)(A->private)).values[k];  /*take value[k]*/
                return A_ij;
            }
        }

        A_ij = 0.0;
        return A_ij;
    }

}

void spmat_print(const struct _spmat *A, int nnz, int n){
    int i;
    printf("values:\n");
    printf("[");
    for (i = 0; i < nnz; ++i) {
        printf("%.1f, ", (*(spmat_array*)(A->private)).values[i]);

    }
    printf("]\n");

    printf("colind:\n");
    printf("[");
    for (i = 0; i < nnz; ++i) {
        printf("%d, ", (*(spmat_array*)(A->private)).colind[i]);

    }
    printf("]\n");

    printf("rowptr:\n");
    printf("[");
    for (i = 0; i < n+1; ++i) {
        printf("%d, ", (*(spmat_array*)(A->private)).rowptr[i]);

    }
    printf("]\n");
}



