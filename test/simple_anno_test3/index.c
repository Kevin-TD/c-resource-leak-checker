#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"


void does_free(char* s Calls("free"), char* p Calls("free")) {
    free(s);
    free(p);
}

int main() {
    char* s = (char*)malloc(15);
    char* p = (char*)malloc(15);
    does_free(s, p);
}