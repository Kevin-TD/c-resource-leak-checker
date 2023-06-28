#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
   char *str;

   /* Initial memory allocation */
   str = (char *) malloc(15);
   strcpy(str, "helloworld");
   printf("String = %s,  Address = %u\n", str, str);

   /* Reallocating memory */
   str = (char *) realloc(str, 25);
   strcat(str, "hello");
   printf("String = %s,  Address = %u\n", str, str);

   free(str);
   
   return(0);
}

/*
Results: 

CalledMethods entry %retval {}
MustCall entry 

entry %str {free}
entry %retval {}

Format: [branch name] [var name] [called methods]


*/