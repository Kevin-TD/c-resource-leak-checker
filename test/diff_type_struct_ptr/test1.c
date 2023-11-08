#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

typedef struct Pair1 {
    char* x MustCall("free"); 
    char* y MustCall("free");
} Pair1;

typedef struct Pair2 {
    char* y MustCall("free");
    char* x MustCall("free"); 
} Pair2;

// Pair1 == Pair2 

int main() {   
    Pair1 p;
    Pair2* ref = &p; 
    ref->x = (char*)malloc(15); 
    ref->y = (char*)malloc(15); 
    
    strcpy(ref->x, "str1");
    strcpy(ref->y, "str2");

    printf("%s %s\n", p.x, p.y);  // str2 str1

    free(ref->x);
    free(ref->y); 
    printf("this is legal!\n");

    // ref->x aliased with p.y, and ref->y aliased with p.x 
    
}