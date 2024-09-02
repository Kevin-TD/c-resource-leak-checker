#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

// function return case

char* MustCall("free") foo() { // no warning
    char* x = (char*)malloc(15);
    char* y = (char*)malloc(15);

    if (getchar()) {
        return x;
    } else {
        return y;
    }
}