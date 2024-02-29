#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

typedef struct my_struct {
    char* a MustCall("free");
    char* b MustCall("free");
} my_struct;

void calls_all(my_struct S Calls("free", "a") Calls("free", "b")) {
    free(S.a);
    free(S.b);
}

int main() {
    my_struct test; 
    test.a = (char*)malloc(15);
    test.b = (char*)malloc(15);
    calls_all(test);

    return 0; 
}