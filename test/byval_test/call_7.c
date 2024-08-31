#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"


typedef struct my_struct {
    char* a MustCall("free");
    char* b MustCall("free");
    char* c MustCall("free");
    char* d MustCall("free");
    char* e MustCall("free");
    char* f MustCall("free");
    char* g MustCall("free");
} my_struct;


void calls_all(my_struct S Calls("free", "a")
               Calls("free", "b")
               Calls("free", "d")
               Calls("free", "c")
               Calls("free", "e")
               Calls("free", "f")
               Calls("free", "g")) {
    free(S.a);
    free(S.b);
    free(S.c);
    free(S.d);
    free(S.e);
    free(S.f);
    free(S.g);
}


void calls_all_but_f(my_struct S Calls("free", "a")
                     Calls("free", "b")
                     Calls("free", "d")
                     Calls("free", "c")
                     Calls("free", "e")
                     Calls("free", "g")) {
    free(S.a);
    free(S.b);
    free(S.c);
    free(S.d);
    free(S.e);
    free(S.g);
}


int main() {
    my_struct testA;
    testA.a = (char*)malloc(15);
    testA.b = (char*)malloc(15);
    testA.c = (char*)malloc(15);
    testA.d = (char*)malloc(15);
    testA.e = (char*)malloc(15);
    testA.f = (char*)malloc(15);
    testA.g = (char*)malloc(15);
    calls_all(testA);


    my_struct testB;
    testB.a = (char*)malloc(15);
    testB.b = (char*)malloc(15);
    testB.c = (char*)malloc(15);
    testB.d = (char*)malloc(15);
    testB.e = (char*)malloc(15);
    testB.g = (char*)malloc(15);
    calls_all_but_f(testB);


    return 0;
}