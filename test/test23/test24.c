#include "test23.h"

// same as test23.c but without annotating again

// the annotations indeed pop up again 

// i don't think we'll need to generate a json or anything 
// like that since the annotation pops up again.
// the include files are also loaded in again so we can also choose to read
// either from the .h declaration or the .c impl. 

void does_free(char* s) { 
    free(s); 
}

char* creates_obligation(char* s, struct my_struct X) {    
    free(s);  
    free(X.x); 
    char* str = (char*)malloc(15); 
    return str; 
}

struct my_struct does_something(struct my_struct S) {
    free(S.x); 
    return S; 
}

int main() {
    // should we have to worry about cases where structs are defined 
    // inside a method body? the AST cannot easily extract annotations
    // when the struct is inside a method body (we to go inside the method).

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