#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int otherFunc(char* str) {
    int a = getchar(); 

    if (a == -15) { // impossible

    }
    else if (a == -10) { // impossible
        
    }
    else {
        free(str);
    }
}

int main () {
   char *str;
   int a = getchar();

   str = (char *) malloc(15);
   strcpy(str, "helloworld");
   printf("String = %s,  Address = %u\n", str, str);


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

   printf("String = %s,  Address = %u\n", str, str);


    
    // call methods pass should say that str does not have its must call satisfied, since it is not the case that in all its branches, free(str) is called. the consistency pass should conclude that it does have it satisfied, as the first two branches are impossible cases 
   
   
   return(0);
}