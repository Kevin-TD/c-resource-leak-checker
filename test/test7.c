#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* malloc0(size_t __size) {
    return malloc(__size);
}

void* malloc1(size_t __size) {
    return malloc(__size);
}

void* malloc2(size_t __size) {
    return malloc(__size);
}

void* malloc3(size_t __size) {
    return malloc(__size);
}

void free0(void* p) {
    free(p);
}

void free1(void* p) {
    free(p);
}

void free2(void* p) {
    free(p);
}

void free3(void* p) {
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

    if (a == -15) { // entry, preds = {}, B0
        free0(str);
        free(str);
        char* m; 
        m = (char*)malloc(15); 
        free(m);
        if (a == -200) {
            free0(m);
        }
        else {
            free0(m);
        }
    }
    else {
        free(str);
    }

   return(0);

 
}

/*
Results (ALLOW_REDEFINE set true)
if.end11 str {free}
if.end m {free, free0}
if.end str {free, free0}

*/