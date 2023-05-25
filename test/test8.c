#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this is a test more for the consistency pass 
// should be able to conclude that trivially_malloc is effectively the same as malloc

void* trivially_malloc(size_t __size) {
    malloc(__size);
}

int main () {
   char *str;
   int a = getchar();

   /* Initial memory allocation */
   str = (char *) malloc(15);
   strcpy(str, "helloworld");
   printf("String = %s,  Address = %u\n", str, str);

   /* Reallocating memory */
   str = (char *) realloc(str, 25);
   strcat(str, "hello");
   printf("String = %s,  Address = %u\n", str, str);


    if (a == -15) { // impossible

    }
    else if (a == -10) { // impossible
        char* str1; 
        str1 = (char *) malloc(15);

        if (a == -9) { // very impossible 
            free(str1);
        } else {
            free(str1);
        }

    }
    else {
        free(str);
    }
    // call methods pass should say that str does not have its must call satisfied, since it is not the case that in all its branches, free(str) is called. the consistency pass should conclude that it does have it satisfied, as the first two branches are impossible cases 
   
   
   return(0);
}