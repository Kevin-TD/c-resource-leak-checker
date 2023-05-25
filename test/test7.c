#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main () {
   char *str;
   int a = getchar();

   str = (char *) malloc(15);
   strcpy(str, "helloworld");
   printf("String = %s,  Address = %u\n", str, str);

   free(str);


    // for called methods, if any single one of these branches does not satisfy a must call, then we conclude at the end that must call is not satisfied 
   if (a == -15) { // impossible

    }
    else if (a == -10) { // impossible
        if (a == -5) {

        }
        else if (a == -100) {

        }
        else {
            realloc(str, 25);
        }
        
    }
    else {
        free(str);
    }

    // expectations: must call not satisfied 
    // reasoning: if in any branch (even if we know it is not possible to reach it) an unsafe method is called, the must call should no longer be satisfied 


   
   return(0);
}