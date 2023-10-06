#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strcpy(char *__restrict__ __dest, const char *__restrict__ __src) {
    printf("a mimicry");
}

int main () {
   char *str;
   int a;

   /* Initial memory allocation */
   str = (char *) malloc(15);
   strcpy(str, "helloworld");
   printf("String = %s,  Address = %u\n", str, str);

   /* Reallocating memory */
   str = (char *) realloc(str, 25);
   strcat(str, "hello");
   printf("String = %s,  Address = %u\n", str, str);

   free(str);
   
   strcpy(str, "helloworld");
   
   return(0);
}