#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free");
    char* y MustCall("free");
};

void reassignPointer(char* s MustCall("free"), struct M m MustCall("free", "y")) {
    s = (char*)malloc(15);
    m.y = (char*)malloc(15);
}

int main() {
    struct M s, s1, s2;
    char* s3;

    s.x = (char*)malloc(15);

    s.y = s.x;
    s1.y = s.x;
    s2.y = s.x;
    s3 = s.x;

    reassignPointer(s.y, s1);

    free(s.x);
}