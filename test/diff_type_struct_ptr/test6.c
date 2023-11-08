#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

typedef struct Pair1 {
    char* y MustCall("free");
    char* x MustCall("free"); 
} Pair1;

typedef struct Pair3 {
    int a; 
    int b; 
    int c; 
    char* letters MustCall("free"); 
} Pair3;


int main() {   
    Pair1 p;
    p.x = (char *)malloc(15);
    p.y = (char *)malloc(15);
    strcpy(p.x, "p.x");
    strcpy(p.y, "p.y");

    printf("%s\n", p.y);
    printf("sizeof Pair1 = %lu, sizeofPair3 = %lu\n", sizeof(Pair1), sizeof(Pair3)); // 16, 24

    Pair3* ref = &p; 

    strcpy(ref->letters, "p.y2"); // seg fault
    printf("%s %s %s\n", p.x, p.y, ref->letters); 

    free(ref->letters);

    printf("this NOT is legal!\n");
    

}