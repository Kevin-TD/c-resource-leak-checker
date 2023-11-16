#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

typedef struct my_struct {
    char* x MustCall("free"); 
} my_struct; 


char* Calls("free") return_calls_free(char* S Calls("free")) {
    free(S); 
    return S; 
}

my_struct Calls("free", "x") return_calls_free_on_struct(my_struct S Calls("free", "x")) {
    free(S.x); 
    return S; 
}



int main() {
    char* var1 = (char*)malloc(15); 
    char* var1_freed = return_calls_free(var1); 

    my_struct var2; 
    var2.x = (char*)malloc(15);
    my_struct var2_freed = return_calls_free_on_struct(var2); 



}