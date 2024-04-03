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

    if (free0(str)) {

        // if.then, preds = {entry}
    } else {

    }

    // if.end, preds = {if.else, if.then}
    if (a == -15) {

        // if.then3, preds = {if.end}
    } else if (free1(str)) {

        // if.then7, preds = {if.else4}
    }

    // if.end9, preds = {if.end8, if.then3}
    while (free2(str)) {

    }



    // while.end, preds = {while.cond}
    return(0);


}

/*
Results
while.end str {free0, free2}


*/