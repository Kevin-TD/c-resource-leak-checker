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
   char *str;
   int a = getchar();

   str = (char *) malloc(15);
   strcpy(str, "helloworld");
   printf("String = %s,  Address = %u\n", str, str);

   str = (char *) realloc(str, 25);
   strcat(str, "hello");
   printf("String = %s,  Address = %u\n", str, str);  

    while (a < 10) {
        while (a < 15) {
            free(str);
        }

        for (int i = 0; i < 15; i++) {
            free(str);
        }

        if (a == -15) {
            free(str);
        }
        a++; 

        switch (a) {
            case 0:
                free(str);
                break; 
            case 1: 
                free(str); 
                break;
            default:
                free(str);
        }
        
    }

    free0(str);
    

   return(0);

 
}

/*
Results 
sw.epilog str {free}
while.end14 str {free0}

*/