#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free, free1");
    char* y MustCall("free");
};

void free1(void* p Calls("free1")) {
    free(p);
}

int main() {
    struct M s;

    free(s.x);
    free1(s.x);
}