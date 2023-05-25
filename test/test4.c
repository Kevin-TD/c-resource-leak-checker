#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// if there is a re-definition of a critical function, automatically cast it as an unsafe function. if it was casted as anything else, delete them 
// LLVM IR can't seem to distinguish between a call for malloc from C library or one we just locally defined 
// additionally, the code itself calls the locally defined malloc function anyhow 

// running the code with the real malloc causes no error
// running the code with the fake malloc causes seg fault; i think this is suitable grounds to then simply cast it as an unsafe function 

// perhaps we'll report an error whenever critical functions are re-defined  

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

   // expectations: %str says must call is not satisfied 
   
   return(0);
}