#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

char* MustCall("free") foo() { // no annotation warning
    char* x = (char*)malloc(15);
    char* y = (char*)malloc(15);

    if (getchar()) {
        return x; // note: should warn that y is leaked
    } else {
        return y; // note: should warn that x is leaked
    }
}