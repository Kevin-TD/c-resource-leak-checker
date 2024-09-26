#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free");
    char* y MustCall("free");
    char* z MustCall("free");
    char* z1 MustCall("free");
};

struct M MustCall("free", "x")
MustCall("free", "y")
MustCall("free", "z")
MustCall("free", "z1")
m_init() {                  // no warning
    struct M m;
    m.x = (char*)malloc(15);
    m.y = (char*)malloc(15);
    m.z = (char*)malloc(15);
    m.z1 = (char*)malloc(15);

    return m;
}

struct M m_init2() { // warning - missing MustCall("free", "x"), MustCall("free", "y"), MustCall("free", "z"), MustCall("free", "z1")
    struct M m;
    m.x = (char*)malloc(15);
    m.y = (char*)malloc(15);
    m.z = (char*)malloc(15);
    m.z1 = (char*)malloc(15);

    return m;
}