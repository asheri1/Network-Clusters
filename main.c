#include <stdio.h>
#include <stdlib.h>
#include "spmat.h"
#include "algorithms.h"
#include "A.h"
#include "B_hat.h"
#include "linked_list_m.h"
#include "checks.h"

void open_file(FILE* file, int* V, int* M, int** degrees, spmat** matrix);
void write_result(linkedlist** O, FILE* graph_out);

int main(int argc, char* argv[]) {
    int V, M;
    int* degrees;
    spmat* matrix;
    FILE* graph_in;
    FILE* graph_out;
    linkedlist* O;
    A* a;

    (void)argc;

    /* Load the graph file attached to the program and check if the operation was performed correctly */
    graph_in = fopen(argv[1], "r" );
    check_file_allocate(&graph_in);

    /* Open the file and collect the data needed to allocate the struct A *
    * Who represented the graph in our program */
    open_file(graph_in, &V, &M, &degrees, &matrix);
    fclose(graph_in);

    /* Allocate the struct A and check if the allocation was performed correctly */
    a = A_allocate(V, M, degrees, matrix);
    check_allocation_A(&a);

    /* Allocate the linked list that will contain later in the plan the division of
     * the graph and check if the allocation was performed correctly */
    O = linkedlist_allocate();
    check_allocation_ll(&O);

    /* The function that performs the division for the best modularity which gets as a parameters a representation
     * of the graph it is dividing and a group O that at the end of the call will contain the division */
    algorithm3(a, &O);

    /* Open the file to which we will write the partition and check if we did it correctly */
    graph_out = fopen(argv[2], "w+");
    check_opening_file(&graph_out);

    /* Write the contained partition in O to the output file */
    write_result(&O, graph_out);
    fclose(graph_out);

    a->free(a);
    free(a);

    return 0;
}

void write_result(linkedlist** O, FILE* graph_out){
    int len;
    int n_g, k;
    int* g;
    linkedlist* head;
    len = (*O)->length;
    head = *O;

    k = fwrite(&len, sizeof(int),1 , graph_out);
    check_writing_size(&k, 1);

    while(0 != ((*O)->group_size)){

        g = (*O)->group;
        n_g = (*O)->group_size;

        k = fwrite(&n_g, sizeof(int),1 , graph_out);
        check_writing_size(&k, 1);

        k = fwrite(g, sizeof(int),n_g , graph_out);
        check_writing_size(&k, n_g);

        (*O) = (linkedlist *) (*O)->next;
    }

    delete_ll(head);

}

/* The function opens the file and collects from it all the data needed to represent the attached graph as a struct A*
 * such as number of vertices in the graph 'V', total number of degrees in the graph 'M' and a list of all the degrees */
void open_file(FILE* file, int* V, int* M, int** degrees, spmat** matrix) {
    int i, j, curr_deg, k, counter;
    double *buffer_to_spmat;
    int *buffer_from_file;

    /* First iteration over the input file to calculate V, M and the degrees array */
    fread(V, sizeof(int), 1, file);     /* Read the number of vertices in the graph 'V' from file */
    check_divide_by_zero(V);

    *degrees = calloc((*V),sizeof(int));
    check_allocation_int(degrees);

    /* Calculating total number of degrees of vertices in the graph  */
    while (fread(&curr_deg, sizeof(int), 1, file) == 1) {
        *M += curr_deg;
        **degrees = curr_deg;
        (*degrees)++;
        k = fseek(file, curr_deg * sizeof(int), SEEK_CUR);

        check_reading_size(&k, 0);
    }

    (*degrees) -= (*V);     /* Set pointer of degrees array back to start */
    k = fseek(file, 2 * sizeof(int), SEEK_SET);     /* Going back to the start of the file so we can start a new iteration */
    check_reading_size(&k, 0);

    /* Initialize spars matrix struct for the adjacency matrix of the graph */
    *matrix = spmat_allocate_array(*V, *M);
    buffer_to_spmat = calloc(*V, sizeof(double));
    check_allocation_double(&buffer_to_spmat);

    /* Second iteration over the input file to calculate the adjacency matrix */
    for (i = 0; i < *V; ++i) {
        curr_deg = (*degrees)[i];   /* The degree of the vertex were running on his neighbors */

        /* If the i'th vertex in the graph has neighbors, add them to the adjacency matrix */
        if (curr_deg != 0) {
            buffer_from_file = calloc(curr_deg, sizeof(int));
            check_allocation_int(&buffer_from_file);

            k = fread(buffer_from_file, sizeof(int), curr_deg, file);       /* Reading into buffer the vertices our current vertex has neighbors with */
            check_reading_size(&k, curr_deg);

            /* Writing into buffer the i'th row of the adjacency matrix of A */
            counter = 0;
            for (j = 0; j < *V; ++j) {
                if (j == *buffer_from_file) {
                    buffer_to_spmat[j] = 1.0;

                    if(counter < curr_deg){
                        buffer_from_file++;
                        counter++;
                    }

                } else {
                    buffer_to_spmat[j] = 0.0;
                }
            }

            /* Adding the i'th row into A's spars matrix struct */
            (*matrix)->add_row((*matrix), buffer_to_spmat, i);
            buffer_from_file -= curr_deg;

            free(buffer_from_file);
            k = fseek(file, 1 * sizeof(int), SEEK_CUR);
            check_reading_size(&k, 0);

        }

            /* If the i'th vertex in the graph has no neighbors add an empty row to the spars matrix */
        else{
            for (j = 0; j < *V; ++j) {
                buffer_to_spmat[j] = 0;
            }
            (*matrix)->add_row((*matrix), buffer_to_spmat, i);
            fseek(file, 1 * sizeof(int), SEEK_CUR);
        }
    }
    free(buffer_to_spmat);
}
