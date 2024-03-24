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

    if (a == -15) {
        // if.then, preds = {entry}
        free0(str);
        free(str);
        char* m;
        m = (char*)malloc(15);
        free(m);
        if (a == -200) {
            // if.then9, preds = {if.then}
            free0(m);
        } else {
            // if.else, preds = {if.then}
            free0(m);
        }
        // if.end, preds = {if.else, if.then9}
    } else {
        // if.else10, preds = {entry}
        free(str);
    }

    // if.end11, preds = {if.else10, if.end}
    return(0);
}