#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Annotations/Annotations.h"

void* MustCall("free") malloc0(size_t __size) {
    return malloc(__size);
}

void* MustCall("free") malloc1(size_t __size) {
    return malloc(__size);
}

void* MustCall("free") malloc2(size_t __size) {
    return malloc(__size);
}

void* MustCall("free") malloc3(size_t __size) {
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
   char *str;
   int a = getchar();

   str = (char *) malloc(15);

    // int i = 0; i < ... ; i++
   for (free0(str); free1(str); free2(str)) {

   }

   return(0);

 
}

/*
Results: 
for.end str {free0, free1}

*/