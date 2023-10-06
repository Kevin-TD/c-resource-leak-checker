#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

int main() {
    // malloc 
    char* str; 
    str = (char*)malloc(15);

    // calloc 
    int n = 5;
    int* numbers = (int*)calloc(n, sizeof(int));
    

    // valloc
    size_t pageSize = sysconf(_SC_PAGESIZE);
    void* alignedMemory = valloc(pageSize);
    printf("valloc Allocated memory address: %p\n", alignedMemory);

    // aligned_alloc
    size_t alignment = 16;
    size_t size = 32;
    void* alignedBlock = aligned_alloc(alignment, size);
    printf("aligned_alloc Allocated memory address: %p\n", alignedBlock);

    free(str);
    free(numbers);
    free(alignedMemory);
    free(alignedBlock);

    char* str2;
   char** p = &str2; 
   char*** x = &p;

   *p = (char*)malloc(15);
   free(**x);



    return 0; 

}