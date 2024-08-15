#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free");
    char* y MustCall("free");
};

int main() {
    struct M s, s1;

    s.x = (char*)malloc(15);

    s.y = s.x; // aliased
    s1.y = s.x; // aliased

    s.y = (char*)malloc(15); // s.y no longer aliased with s.x and s1.y

    free(s.x); // s.x and s1.y freed, s.y not freed
}