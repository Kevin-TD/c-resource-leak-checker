#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
         char* s; 
         s = (char*)malloc(15);
         free(s);
    }

   free(str);
   return(0);

 
}