#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

typedef struct Pair1 {
    char* x MustCall("free"); 
    char* y MustCall("free");
} Pair1;

typedef struct Pair4 {
    char* x MustCall("free"); 
    char* y MustCall("free"); 
    char* z MustCall("free"); 
} Pair4;

// Pair4 != Pair1, sizeof(Pair4) > sizeof(Pair1)

int main() {   
    Pair1 p;
    Pair4* ref = &p; 
    ref->x = (char*)malloc(15); 
    ref->y = (char*)malloc(15); 
    free(ref->x);
    free(ref->y); 
    printf("this is legal!\n");

}