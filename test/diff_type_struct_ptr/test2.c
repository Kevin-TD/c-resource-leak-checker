#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

typedef struct Pair1 {
    char* x MustCall("free"); 
    char* y MustCall("free");
} Pair1;

typedef struct Pair3 {
    char* X MustCall("free"); 
} Pair3;

// Pair3 != Pair1, sizeof(Pair3) < sizeof(Pair1)


int main() {   
    Pair1 p;
    p.x = (char *)malloc(15);
    p.y = (char *)malloc(15);
    strcpy(p.x, "p.x");
    strcpy(p.y, "p.y");

    printf("%s\n", p.x);
    printf("sizeof Pair1 = %lu, sizeofPair3 = %lu\n", sizeof(Pair1), sizeof(Pair3)); // 16, 8 

    Pair3* ref = &p; 
    strcpy(ref->X, "p.x2");
    printf("%s %s\n", p.x, ref->X);

    free(ref->X);
    free(p.y);

    printf("this is legal!\n");

    // ref->X and p.x are aliased 
    

}