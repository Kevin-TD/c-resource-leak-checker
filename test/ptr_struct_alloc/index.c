#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

typedef struct my_struct {
    char* A;
    char* B;
    char* C;
} my_struct;

void does_free_one_pointer(my_struct* m Calls("free", "A") Calls("free", "B") Calls("free")) {
    free(m->A);
    free(m->B);
    free(m);
}


int main() {
    my_struct* my_struct_ptr = malloc(sizeof(my_struct));
    my_struct_ptr->A = (char*)malloc(15);
    my_struct_ptr->B = (char*)malloc(15);

    does_free_one_pointer(my_struct_ptr);

    my_struct* my_struct_ptr_normal_free = malloc(sizeof(my_struct));
    my_struct_ptr_normal_free->A = (char*)malloc(15);
    my_struct_ptr_normal_free->B = (char*)malloc(15);

    free(my_struct_ptr_normal_free);
    free(my_struct_ptr_normal_free->A);
    free(my_struct_ptr_normal_free->B);
}