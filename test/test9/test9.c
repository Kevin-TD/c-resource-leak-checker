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

void free0(void* p Calls("free0")) {
    free(p);
}

void free1(void* p Calls("free1")) {
    free(p);
}

void free2(void* p Calls("free2")) {
    free(p);
}

void free3(void* p Calls("free3")) {
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

    // while.cond, preds = {sw.epilog, entry}
    while (a < 10) {
        // while.body, preds = {while.cond}
        while (a < 15) {
            // while.body9, preds = {while.cond7}
            free(str);
        }

        // while.end, preds = {while.cond7}
        for (int i = 0; i < 15; i++) {
            // for.body, preds = {for.cond}
            free(str);
        }

        // for.end, preds = {for.cond}
        if (a == -15) {
            // if.then, preds = {for.end}
            free(str);
        }
        // if.end, preds = {if.then, for.end}
        a++;

        switch (a) {
        case 0:
            // sw.bb, preds = {if.end}
            free(str);
            break;
        case 1:
            // sw.bb13, preds = {if.end}
            free(str);
            break;
        default:
            // sw.default, preds = {if.end}
            free(str);
        }

    }

    // while.end14, preds = {while.cond}
    free0(str);


    return(0);


}

/*
Results
sw.epilog str {free}
while.end14 str {free0}

*/