#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free");
    char* y MustCall("free");
};

void reassignPointer(char* s MustCall("free")) {
    s = (char*)malloc(15);
}

int main() {
    struct M s;
    char* s1;

    s.x = (char*)malloc(15);

    s.y = s.x;
    s1 = s.x; // s1, s.x, s.y aliased

    printf("%s", s1);

    reassignPointer(s1); // s1 no longer aliased

    free(s.x); // s.x, s.y freed. s1 not
}