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

    a.a = set_mc1(a.a);
    a.a = set_mc2(a.a); // Warning -- missing CalledMethods("set_mc1") on param 0 & missing MustCall("foo1") on param 0

    b.a = set_mc1(b.a);

    b.a = a.a; // Warning -- a.a input set larger than b.a input set. missing MustCall("foo2") on b.a


}
