
/*module that throws exception if an error occurred in memory*/


#ifndef CLUSTER_CHECKS_H
#define CLUSTER_CHECKS_H

#include "A.h"
#include "B_hat.h"
#include "linked_list_m.h"

/* Receives a pointer to an array of integers and checks whether a memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_allocation_int(int** pointer);

/* Receives a pointer to an array of doubles and checks whether a memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_allocation_double(double** pointer);

/* Receives a pointer to a type A struct and checks whether the memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_allocation_A(A** pointer);

/* Receives a pointer to a type B_hat struct and checks whether the memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_allocation_B(B_hat** pointer);

/* Receives a pointer to a type linked list struct and checks whether the memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_allocation_ll(linkedlist** pointer);

/* Receives a pointer to a type FILE and checks whether the memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_file_allocate(FILE** pointer);

/* Receives a pointer to the file and checks if it is opened correctly */
/* Otherwise returns an error message and exits the program */
void check_opening_file(FILE** pointer);

/* Receives a pointer to an integer and an integer and checks if they are equal */
/* Otherwise returns an error message and exits the program */
void check_reading_size(int* var, int size);

/* Receives a pointer to an integer and an integer and checks if they are equal */
/* Otherwise returns an error message and exits the program */
void check_writing_size(int* var, int size);

/* Gets a number and checks if it is equal to zero */
/* Otherwise returns an error message and exits the program */
void check_divide_by_zero(int* num);


#endif
