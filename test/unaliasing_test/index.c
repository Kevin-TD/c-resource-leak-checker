#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free");
    char* y MustCall("free, free1");
};

void free1(void* p Calls("free1")) {
    free(p);
}

int main() {
    struct M s;

    s.x = (char*)malloc(15);

    s.y = s.x; // aliased

    s.y = (char*)malloc(15); // no longer aliased

    free(s.x); // s.x freed, s.y not freed

    // TODO: write test pass and implement unaliasing. note the complications with updating with proper must call's when we unalias.
    // and complications with the fact they are aliased at one point then unaliased
}