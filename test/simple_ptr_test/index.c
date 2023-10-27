#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct my_struct {
    char* x MustCall("free"); 
    char* y MustCall("free");
};

int main() {   
    char* var1 = (char*)malloc(15);
    char** var2 = &var1;
    free(*var2); 

    char* var3 = (char*)malloc(15);
    char** var4 = &var3;
    char*** var5 = &var4;
    char**** var6 = &var5;
    free(***var6); 


    //! pointer structs are not being handled correctly 

    struct my_struct M; 

    struct my_struct* M_ptr = &M;
    M_ptr->x = (char*)malloc(15); 
    free(M_ptr->x); 

    struct my_struct** M_ptr_ptr = &M_ptr; 

}