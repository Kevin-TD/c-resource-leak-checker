#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for each entry = ..., preds = {...}  statement, all the code above it (including the line it was declared in) is within that branch up until (and before) another branch declaration (or the declaration of the method)

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
         free(str);
    }

   free(str);
   return(0);

 
}