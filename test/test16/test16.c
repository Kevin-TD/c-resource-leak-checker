#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

//! So, even though our annotations currently aren't ideal, LLVM IR still interprets them. This created more IR code & calls to llvm.annotations and aliases between IR variables, and I don't find it necessary to handle it right now when we are not relying on the IR for annotations. Thus, all annotations have been moved to the main method, where the IR won't do anything extra 

struct my_struct {
    char* x MustCall("free");
    char* y MustCall("free");
};

typedef struct struct_2 {
    char* x MustCall("free");
    char* y MustCall("free");
} my_struct_2;

void does_free(char* s MustCall("free")) { 
    free(s); 
}


char* MustCall("free") creates_obligation(char* s Calls("free"), struct my_struct X Calls("free", "x")) {    
    free(s);  
    free(X.x); 
    char* str = (char*)malloc(15); 
    return str; 
}



struct my_struct Calls("free", "x") does_something(struct my_struct S Calls("free", "x")) {
    free(S.x); 
    return S; 
}


int main() {
    struct my_struct_3 {
        char* x MustCall("free");
        char* y MustCall("free");
    };

    struct my_struct k; 
    my_struct_2 m;
    struct my_struct_3 km; 

    k.x = (char*)malloc(15); 
    k.y = (char*)malloc(15); 
    // obligations to free k.x and k.y created 

    m.y = (char*)malloc(15); 
    km.x = (char*)malloc(15); 

    char* s = (char*)malloc(15);  
    char* y = creates_obligation(s, k);  // creates obligation to free y; the method itself also calls free on k.x 

    free(y); // y, k.x freed. now to free k.y 
    does_free(k.y); // k.y freed 

    does_free(m.y);
    does_free(km.x);
    
    // all obligations satisfied 

}