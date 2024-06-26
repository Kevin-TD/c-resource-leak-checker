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

int free0(void* p Calls("free")) {
    free(p);
    return 1;
}

int free1(void* p Calls("free")) {
    free(p);
    return 1;
}

int free2(void* p Calls("free")) {
    free(p);
    return 1;
}

int free3(void* p Calls("free")) {
    free(p);
    return 1;
}



int main () {
    // entry
    char *str;
    int a = getchar();

    str = (char *) malloc(15);

    // int i = 0; i < ... ; i++
    // for.cond, preds = {for.inc, entry}
    for (free0(str); free1(str); free2(str)) {

        // for.body, preds = {for.cond}
    }

    // for.end, preds = {for.cond}
    return(0);


}

/*
Results:
for.end str {free0, free1}

*/