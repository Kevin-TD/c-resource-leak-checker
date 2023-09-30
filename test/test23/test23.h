#include "../../Annotations/Annotations.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct my_struct {
  char *x MustCall("free");
  char *y MustCall("free");
};

typedef struct struct_2 {
  char *x MustCall("free");
  char *y MustCall("free");
} my_struct_2;

struct my_struct_3 {
  char *x MustCall("free");
  char *y MustCall("free");
};

void does_free(char *s MustCall("free"));

char *MustCall("free")
    creates_obligation(char *s Calls("free"),
                       struct my_struct X Calls("free", "x"));

struct my_struct Calls("free", "x")
    does_something(struct my_struct S Calls("free", "x"));