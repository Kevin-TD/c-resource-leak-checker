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

int free0(void* p) {
    free(p);
    return 1; 
}

int free1(void* p) {
    free(p);
    return 1;
}

int free2(void* p) {
    free(p);
    return 1;
}

int free3(void* p) {
    free(p);
    return 1; 
}



int main () {
   char* str;

   char** p = &str; 
   char*** x = &p;

   *p = (char*)malloc(15);

   strcat(*p, "hello");
   printf("String = %s,  Address = %u\n", *p, *p);


    free(**x);

   return(0);

 
}