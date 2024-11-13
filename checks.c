#include <stdio.h>
#include <stdlib.h>
#include "A.h"
#include "B_hat.h"
#include "linked_list_m.h"

/* Receives a pointer to an array of integers and checks whether a memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_allocation_int(int** pointer){
    if(NULL == *pointer){
        printf("An error occurred while allocating memory");
        exit(EXIT_FAILURE);
    }
}

/* Receives a pointer to an array of doubles and checks whether a memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_allocation_double(double** pointer){
    if(NULL == *pointer){
        printf("An error occurred while allocating memory");
        exit(EXIT_FAILURE);
    }
}

/* Receives a pointer to a type A struct and checks whether the memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_allocation_A(A** pointer){
    if(NULL == *pointer){
        printf("An error occurred while allocating memory for A");
        exit(EXIT_FAILURE);
    }
}

/* Receives a pointer to a type B_hat struct and checks whether the memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_allocation_B(B_hat** pointer){
    if(NULL == *pointer){
        printf("An error occurred while allocating memory for B");
        exit(EXIT_FAILURE);
    }
}

/* Receives a pointer to a type linked list struct and checks whether the memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_allocation_ll(linkedlist** pointer){
    if(NULL == *pointer){
        printf("An error occurred while allocating memory");
        exit(EXIT_FAILURE);
    }
}

/* Receives a pointer to a type FILE and checks whether the memory allocation has been performed correctly */
/* Otherwise returns an error message and exits the program */
void check_file_allocate(FILE** pointer){
    if(NULL == *pointer){
        printf("An error occurred while allocating memory");
        exit(EXIT_FAILURE);
    }
}

/* Receives a pointer to the file and checks if it is opened correctly */
/* Otherwise returns an error message and exits the program */
void check_opening_file(FILE** pointer){
	if(NULL == *pointer){
	    printf("An error occurred while opening the file");
	    exit(EXIT_FAILURE);
	}
}
/* Receives a pointer to an integer and an integer and checks if they are equal */
/* Otherwise returns an error message and exits the program */
void check_reading_size(int* var, int size){
    if ( *var != size){
        printf("An error occurred while reading from the file");
        exit(EXIT_FAILURE);
    }
}

/* Receives a pointer to an integer and an integer and checks if they are equal */
/* Otherwise returns an error message and exits the program */
void check_writing_size(int* var, int size){
    if ( *var != size){
        printf("An error occurred while writing to the file");
        exit(EXIT_FAILURE);
    }
}
/* Gets a number and checks if it is equal to zero */
/* Otherwise returns an error message and exits the program */
void check_divide_by_zero(int* num){
    if(0 == *num){
        printf("The number of nodes in the graph is zero so there is a danger of dividing by zero later in the program");
        exit(EXIT_FAILURE);
    }
}