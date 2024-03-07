#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

typedef struct my_struct {
    char* A;
    char* B;
    char* C;
} my_struct;

typedef my_struct***** a_lot_of_ptr;
typedef my_struct* single_ptr;
typedef single_ptr single_ptr_2;

void struct_no_ptr_params(struct my_struct m Calls("free", "A") Calls("free", "B")) {

}

void does_free_one_pointer(single_ptr_2 m Calls("free", "A") Calls("free", "B")) {
    free(m->A);
    free(m->B);
    free(m);
}


void absurd_pointer(a_lot_of_ptr m Calls("free", "C") Calls("free", "B")) {

}

int main() {
    my_struct* my_struct_ptr = malloc(sizeof(my_struct)); // memory allocation for struct
    my_struct_ptr->A = (char*)malloc(15);
    my_struct_ptr->B = (char*)malloc(15);

    does_free_one_pointer(my_struct_ptr);
}