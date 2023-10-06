#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

void does_free(char* s Calls("free")) {
    free(s); 
}

int main() {   
    char* s = (char*)malloc(15); 
    does_free(s); 

    // does_malloc_calls(M);
    // does_free_calls(M);
}