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

    for (int i = 0; i < 5; i++) {
        free(str);
    }
    

   return(0);

 
}