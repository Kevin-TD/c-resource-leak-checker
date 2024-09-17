#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

typedef struct my_struct {
    char* a MustCall("foo_a, foo_b");
    char* b MustCall("foo_a, foo_b");
    char* c MustCall("foo_a, foo_b");
    char* d MustCall("foo_a, foo_b");
    char* filler1 MustCall("foo_a, foo_b");
    char* filler2 MustCall("foo_a, foo_b");
    char* filler3 MustCall("foo_a, foo_b");
    char* filler4 MustCall("foo_a, foo_b");
    char* filler5 MustCall("foo_a, foo_b");
    char* filler6 MustCall("foo_a, foo_b");
    char* filler7 MustCall("foo_a, foo_b");
    char* filler8 MustCall("foo_a, foo_b");
    char* filler9 MustCall("foo_a, foo_b");
    char* filler10 MustCall("foo_a, foo_b");
    char* filler11 MustCall("foo_a, foo_b");
    char* filler12 MustCall("foo_a, foo_b");
    char* filler13 MustCall("foo_a, foo_b");
    char* filler14 MustCall("foo_a, foo_b");
} my_struct;


char* MustCall("foo_a, foo_b") foo_a(char* a) {
    return a;
}

void foo(my_struct S MustCall("foo_a", "a")) {}

int main() {
    my_struct A;
    A.a = foo_a(A.a);
    foo(A); // Warning -- A.a not subset of MustCall("foo_a", "a"). Missing MustCall("foo_b", "a") on foo function

}