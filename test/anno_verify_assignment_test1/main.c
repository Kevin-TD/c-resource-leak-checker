#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

struct A {
    char* a MustCall("foo1");
};

struct B {
    char* a MustCall("foo1, foo2");
};

char* MustCall("foo1") set_mc1(char* a) {
    return a;
}

char* MustCall("foo2") set_mc2(char* a) {
    return a;
}

int main() {
    struct A a;
    struct B b;

    a.a = set_mc1(a.a); // No warning on param 0 of set_mc1 -- a.a is uninitialized
    a.a = set_mc2(a.a); // Warning -- missing MustCall("foo1") on param 0 of set_mc2 + Warning -- MustCall("foo1") (LHS) not superset of MustCall("foo2") (RHS)

    b.a = set_mc1(b.a); // Warning -- missing MustCall("foo1, foo2") on param 0 of set_mc1

    b.a = a.a;


}
