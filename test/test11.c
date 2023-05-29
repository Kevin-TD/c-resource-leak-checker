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
         if (a == -10) { // if.then, preds = entry, B1
            free(str);
         } // if.then8, preds = if.then, B2
         else if (a == -9) { // if.else, preds = if.then, B3
            realloc(str, 15);
         } // if.then10, preds = if.else, B4
         else if (a == -10) { // if.else11, preds = if.else, B5
            printf("%s", str);
            // if.then13, preds = if.else11, B6
         } else { 
            realloc(str, 15);
         } // if.else14, preds = if.else11, B7
        
        // if.end, preds = if.else14, if.then13, B8
        // if.end15, preds = if.end, if.then10, B9

         free(str);

         if (a == -70) {  // if.end16, preds=if.end15, if.then8, B10
            realloc(str, 15);
         } // if.then18, preds = if.end16, B11
        
        free(str);

         // if.end19, preds = if.then18, if.end16, B12


    }
    else if (a == -20) { // if.else20, preds = entry, B13
        realloc(str, 25);
    } // if.then22, preds = if.else20, B14
    else {
        free(str);
    } // if.else24, preds = if.else20, B15
   
   // if.end25, preds = if.else24, if.then22, B16
   
   return(0);

   // if.end26, preds = if.end25, if.end19, 17 
}