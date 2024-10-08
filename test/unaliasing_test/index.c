#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free");
    char* y MustCall("free");
};

int main() {
    struct M s;

    s.x = (char*)malloc(15);

    s.y = s.x; // aliased

    s.y = (char*)malloc(15); // no longer aliased

    free(s.x); // s.x freed, s.y not freed
}