#include "../../Annotations/Annotations.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct my_struct {
    char *x MustCall("free");
    char *y MustCall("free");
} my_struct;

typedef struct struct_2 {
    char *x MustCall("free");
    char *y MustCall("free");
} my_struct_2;

typedef struct my_struct_3 {
    char *x MustCall("free");
    char *y MustCall("free");
} my_struct_3;

void does_free(char *s Calls("free"));

char *MustCall("free")
creates_obligation(char *s Calls("free"),
                   struct my_struct X Calls("free", "x"));

struct my_struct Calls("free", "x")
does_something(my_struct S Calls("free", "x"));