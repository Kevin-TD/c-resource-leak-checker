#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// if x and p are aliased and p calls free, should only say p calls free or that both x and p call free? 


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
   int c = getchar(); 

   *p = (char*)malloc(15);
   strcat(*p, "hello");
   printf("String = %s,  Address = %u\n", *p, *p);


   free(*p);

   return(0);

 
}

/*
Results: 
entry %p {free}

*/