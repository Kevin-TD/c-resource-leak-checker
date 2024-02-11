#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

void* MustCall("free0") malloc0(size_t __size) {
    return malloc(__size);
}

void* MustCall("free1") malloc1(size_t __size) {
    return malloc(__size);
}

void* MustCall("free2") malloc2(size_t __size) {
    return malloc(__size);
}

void* MustCall("free3") malloc3(size_t __size) {
    return malloc(__size);
}

void free0(void* p Calls("free")) {
    free(p);
}

void free1(void* p Calls("free")) {
    free(p);
}

void free2(void* p Calls("free")) {
    free(p);
}

void free3(void* p Calls("free")) {
    free(p);
}

int main () {
    // entry
    char *str;
    int a = getchar();

    str = (char *) malloc(15);
    strcpy(str, "helloworld");
    printf("String = %s,  Address = %u\n", str, str);

    str = (char *) realloc(str, 25);
    strcat(str, "hello");
    printf("String = %s,  Address = %u\n", str, str);

    // for.cond, preds = {for.inc, entry}
    for (int i = 0; i < 10; i++) {
        // for.body, preds = {for.cond}
        free0(str);
    }

    // for.end, preds = {for.cond}
    free0(str);

    if (a < 15) {
        // if.then, preds = {for.end}
        free1(str);
    } else {
        // if.else, preds = {for.end}
        free1(str);
    }

    // if.end, preds = {if.else, if.then}
    return(0);


}