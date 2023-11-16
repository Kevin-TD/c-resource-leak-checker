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

void does_free(char* s Calls("free")) { 
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

char* Calls("free") does_something_simpler(char* S Calls("free")) {
    free(S); 
    return S; 
}


int main() {
    struct my_struct_3 {
        char* x MustCall("free");
        char* y MustCall("free");
    };

    struct my_struct var_k; 
    my_struct_2 m;
    struct my_struct_3 km; 

    var_k.x = (char*)malloc(15); 
    var_k.y = (char*)malloc(15); 

    m.y = (char*)malloc(15); 
    km.x = (char*)malloc(15); 

    char* s = (char*)malloc(15);  
    char* new_y = creates_obligation(s, var_k); 

    free(new_y); 
    does_free(var_k.y);

    does_free(m.y);
    does_free(km.x);

    struct my_struct m_var_1; 
    m_var_1.x = (char*)malloc(15); 
    struct my_struct m_var_2 = does_something(m_var_1); 

    char* simpler = (char*)malloc(15); 
    does_something_simpler(simpler); 

}