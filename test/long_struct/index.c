#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

typedef struct my_struct {
    char* a MustCall("free");
    char* b MustCall("free");
    char* c MustCall("free");
    char* d MustCall("free");
    char* filler1 MustCall("free");
    char* filler2 MustCall("free");
    char* filler3 MustCall("free");
    char* filler4 MustCall("free");
    char* filler5 MustCall("free");
    char* filler6 MustCall("free");
    char* filler7 MustCall("free");
    char* filler8 MustCall("free");
    char* filler9 MustCall("free");
    char* filler10 MustCall("free");
    char* filler11 MustCall("free");
    char* filler12 MustCall("free");
    char* filler13 MustCall("free");
    char* filler14 MustCall("free");
} my_struct;


my_struct Calls("free", "a") does_something_a(my_struct S Calls("free", "a"))
{
    free(S.a);
    return S;
}

my_struct Calls("free", "b") does_something_b(my_struct S Calls("free", "b"))
{
    free(S.b);
    return S;
}

my_struct Calls("free", "c") does_something_c(my_struct S Calls("free", "c"))
{
    free(S.c);
    return S;
}

my_struct Calls("free", "d") does_something_d(int a, my_struct S Calls("free", "d"), my_struct S1 Calls("free", "d"), int c, char* m Calls("free"))
{
    free(S.d);
    return S;
}

int main()
{
    my_struct A;
    A.a = (char*)malloc(15);
    my_struct A1 = does_something_a(A);

    my_struct B;
    B.b = (char*)malloc(15);
    my_struct B1 = does_something_b(B);

    my_struct C;
    C.c = (char*)malloc(15);
    my_struct C1 = does_something_c(C);

    my_struct D, CC;
    D.d = (char*)malloc(15);
    char* m = (char*)malloc(15);
    my_struct D1 = does_something_d(0, CC, D, 2, m);

}