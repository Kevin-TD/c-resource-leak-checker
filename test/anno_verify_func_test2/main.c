#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

char* foo_causes_warning1() {  // warning - missing annotation MustCall("free")
    char* x = (char*)malloc(15);

    if (getchar()) {
        return x;
    } else {
        return x;
    }
}

void func1(char* x) {}

char* foo_causes_warning2() { // warning - missing annotation MustCall("free"), CalledMethods("func1")
    char* x = (char*)malloc(15);
    func1(x);

    if (getchar()) {
        return x;
    } else {
        return x;
    }
}

char* Calls("func1") foo_causes_warning3() { // warning - missing annotation MustCall("free")
    char* x = (char*)malloc(15);
    func1(x);

    if (getchar()) {
        return x;
    } else {
        return x;
    }
}