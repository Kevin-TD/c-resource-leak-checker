#include "test23.h"

int main() {   
    char* s = (char*)malloc(15); 
    char* p = (char*)malloc(15); 
    does_free(s, p); 

    // does_malloc_calls(M);
    // does_free_calls(M);
}