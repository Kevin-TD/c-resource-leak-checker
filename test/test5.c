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
            realloc(str, 15);
        }

    }
    else {
        free(str);
    }

   
   return(0);
}

/*
Results: 
if.end str1 {free}

if.end16 a {}
if.end16 retval {}
if.end16 str {}
if.end16 str1 {}


*/