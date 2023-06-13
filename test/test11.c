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
   char *str;
   int a = getchar();

   str = (char *) malloc(15);

   if (free0(str)) {

   }
   else {

   }

   if (a == -15) {

   } else if (free1(str)) {

   }

   while (free2(str)) {

   }



   return(0);

 
}

/*
Results 
while.end str {free0, free2}


*/