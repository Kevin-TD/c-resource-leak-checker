#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void unknown_function(char* s) {
    printf("do nothing!");
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

   unknown_function(str);

   
   
   return(0);
}

