#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

typedef struct my_struct_1 {
    char* a MustCall("free");
    char* b MustCall("free");
    char* c MustCall("free");
    char* d MustCall("free");
    char* e MustCall("free");
    char* f MustCall("free");
    char* g MustCall("free");
} my_struct_1;

typedef struct my_struct_2 {
    char* a MustCall("free");
    char* b MustCall("free");
    char* c MustCall("free");
    char* d MustCall("free");
    char* e MustCall("free");
    char* f MustCall("free");
    char* g MustCall("free");
} my_struct_2;

typedef struct my_struct_3 {
    char* a MustCall("free");
    char* b MustCall("free");
    char* c MustCall("free");
    char* d MustCall("free");
    char* e MustCall("free");
    char* f MustCall("free");
    char* g MustCall("free");
} my_struct_3;


void calls_d_a_f(my_struct_1 m1 Calls("free", "d") Calls("free", "a") Calls("free", "f"),
                 my_struct_2 m2 Calls("free", "a") Calls("free", "f") Calls("free", "d"),
                 my_struct_3 m3 Calls("free", "f") Calls("free", "d") Calls("free", "a")) {
    free(m1.d);
    free(m1.a);
    free(m1.f);
    free(m2.d);
    free(m2.a);
    free(m2.f);
    free(m3.d);
    free(m3.a);
    free(m3.f);
}

int main() {
    my_struct_1 s1;
    s1.a = (char*)malloc(15);
    s1.d = (char*)malloc(15);
    s1.f = (char*)malloc(15);

    my_struct_2 s2;
    s2.a = (char*)malloc(15);
    s2.d = (char*)malloc(15);
    s2.f = (char*)malloc(15);

    my_struct_3 s3;
    s3.a = (char*)malloc(15);
    s3.d = (char*)malloc(15);
    s3.f = (char*)malloc(15);

    calls_d_a_f(s1, s2, s3);
}