#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

typedef struct M {
    char* x MustCall("free");
    char* y MustCall("free");
} M;

M* m_init() { // warning - missing MustCall("free", "x"), MustCall("free", "y"), MustCall("free")
    M* m = (M*)malloc(sizeof(M));
    m->x = (char*)malloc(15);
    m->y = (char*)malloc(15);

    return m;
}