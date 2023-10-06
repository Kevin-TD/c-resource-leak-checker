#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct my_struct {
    char* x MustCall("free"); 
    char* y MustCall("free");
};

void does_free(char* s Calls("free")) { 
    free(s); 
}

char* MustCall("free") creates_obligation(char* s Calls("free"), struct my_struct X Calls("free", "x")) {    
    free(s);  
    free(X.x); 
    char* str = (char*)malloc(15); 
    return str; 
}


struct my_struct Calls("free", "x") does_something(struct my_struct S) {
    free(S.x); 
    return S; 
} 

void does_malloc_calls(struct my_struct S MustCall("free", "x") MustCall("free", "y")) {
    S.x = (char*)malloc(15);
    S.y = (char*)malloc(15); 
}

void does_free_calls(struct my_struct S Calls("free", "x") Calls("free", "y")) {
    free(S.x);
    free(S.y); 
}


int main() {   
    struct my_struct M; 
    

    // does_malloc_calls(M);
    // does_free_calls(M);
}