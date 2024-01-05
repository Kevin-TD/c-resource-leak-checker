#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free");
    char* y MustCall("free");
};

int main() {
    struct M a;
    struct M b;

    a.x = (char*)malloc(15);

    a.x = b.y;

    free(b.y);
}