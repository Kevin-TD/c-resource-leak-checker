#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../Annotations/Annotations.h"


void does_free(char* s, char* p) {
    free(s); 
    free(p); 
}


Calls(FUNCTION("does_free") AT_PARAMETER("0"), METHODS("free"))
Calls(FUNCTION("does_free") AT_PARAMETER("1"), METHODS("free"))
int main() {   
    char* s = (char*)malloc(15); 
    char* p = (char*)malloc(15); 
    does_free(s, p); 

    // does_malloc_calls(M);
    // does_free_calls(M);
}