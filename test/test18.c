#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* malloc0(size_t __size) {
    return malloc(__size);
}

void* malloc1(size_t __size) {
    return malloc(__size);
}

void* malloc2(size_t __size) {
    return malloc(__size);
}

void* malloc3(size_t __size) {
    return malloc(__size);
}

void free0(void* p) {
    free(p);
}

void free1(void* p) {
    free(p);
}

void free2(void* p) {
    free(p);
}

void free3(void* p) {
    free(p);
}

int main () {
   char *str;
   int a = getchar();

   str = (char *) malloc(15);
   strcpy(str, "helloworld");
   printf("String = %s,  Address = %u\n", str, str);

   str = (char *) realloc(str, 25);
   strcat(str, "hello");
   printf("String = %s,  Address = %u\n", str, str);  

    while (a < 10) {
        free(str); 
        free0(str);

        while (a < 15) {
            free(str);
        }

        for (int i = 0; i < 15; i++) {
            free(str);
        }

        if (a == -15) {
            free(str);
        }
        a++; 

        switch (a) {
            case 0:
                free(str);
                break; 
            case 1: 
                free(str); 
                break;
            default:
                free(str);
        }
        
    }

    free0(str);
    

   return(0);

 
}