#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

// pa = pseudo-assignment

struct M {
    char* a MustCall("foo_a, foo_b");
};

char* MustCall("foo_a, foo_b") foo_a(char* a) {
    return a;
}

void foo(int a, int b, char* c MustCall("foo_a"), int d) {}


int main() {
    struct M m;
    m.a = foo_a(m.a); // Warning -- missing MustCall("foo_a, foo_b") on param 0 of foo_a
    foo(1, 2, m.a, 4); // Warning -- m.a not subset of MustCall("foo_a"). Missing MustCall("foo_b") on foo function
}