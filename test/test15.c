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

   for (int i = 0; i < 15; i++) {
    int b; 
    free(str);
    int c; 

    for (int i = 0; i < 15; i++) {
        int e; 
        free(str);
        int f; 

        if (a == -5) {
            int m; 
        }
        else if (a == -20) {
            int z; 
        }
        else {
            int j; 
        }

        for (int i = 0; i < 15; i++) {
            int g; 
            free(str);  
            int h; 
        } 
    }
   }

   

   return(0);

 
}