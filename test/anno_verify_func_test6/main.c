#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

char* foo1() { // warning -- missing MustCall("free")
    char* retval = (char*)malloc(15);
    if (getchar()) {
        return retval;
    } else {
        return retval;
    }
}