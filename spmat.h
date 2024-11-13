
/*module that defines sparse matrix in memory*/


#ifndef _SPMAT_H
#define _SPMAT_H

typedef struct _spmat {
	/* Matrix size (n*n) */
	int		n;

	/* Adds row i the matrix. Called before any other call,
	 * exactly n times in order (i = 0 to n-1) */
	void	(*add_row)(struct _spmat *A, const double *row, int i);

	/* Frees all resources used by A */
	void	(*free)(struct _spmat *A);

	/* Multiplies matrix A by vector v, into result (result is pre-allocated) */
	void	(*mult)(const struct _spmat *A, const double *v, double *result);

	/* Private field for inner implementation.
	 * Should not be read or modified externally */
	void	*private;

	/* retrieves a value of cell in (i,j) in sparse.  +if cell does not exist/ out of bounds?*/
	double (*get_cell)(const struct _spmat *A, int i, int j);

}spmat;

/* Allocates a new linked-lists sparse matrix of size n */
spmat* spmat_allocate_list(int n);

/* Allocates a new arrays sparse matrix of size n with nnz non-zero elements */
spmat* spmat_allocate_array(int n, int nnz);

void spmat_print(const struct _spmat *A, int nnz, int n);

/*to get the value of certain cell in array*/
double get_cell_array(const struct _spmat *A, int i, int j);



#endif
