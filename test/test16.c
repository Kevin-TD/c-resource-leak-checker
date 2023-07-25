#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../Annotations/Annotations.h"

struct my_struct {
    char* x MustCall("free"); 
    char* y MustCall("free"); 
}; 

void does_free(char* s CalledMethods("free")) {
    free(s); 
}

// return type is something that we must call free on 
/*
   ! now, my_struct has a MustCall on its field x and y. when we pass in the struct here and say it calls free on the struct, how are we encoding it so it calls free on a specific field? 
*/
CalledMethods("something on the header, it should be able to distinguish")
CalledMethods("something else too")
char* MustCall("free") creates_obligation(char* s CalledMethods("free"), struct my_struct CalledMethods("free") X) {
    free(s);  
    free(X.x); 
    char* str = (char*)malloc(15); 
    return str; 
}


int main() {
    struct my_struct k; 
    k.x = (char*)malloc(15); 
    k.y = (char*)malloc(15); 
    // obligations to free k.x and k.y created 

    char* s = (char*)malloc(15);  
    char* y = creates_obligation(s, k);  // creates obligation to free y; the method itself also calls free on k.x 

    free(y); // y, k.x freed. now to free k.y 
    does_free(k.y); // k.y freed 
    
    // all obligations satisfied 

}