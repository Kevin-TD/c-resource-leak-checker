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

void hidden_malloc(char* str) {
    str = (char *)malloc(15); 
}



int main () {
   char* str;
   int a = getchar(); 

   if (a == -15) {
    str = (char *) malloc(15);
   }

   else if (a == -10) {
    str = (char *) malloc2(15);
   }
   else {
    str = (char *) malloc1(15);
   }

   hidden_malloc(str); 




   return(0);

 
}