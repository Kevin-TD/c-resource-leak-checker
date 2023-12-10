#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main () {
    char *str = (char *) malloc(15);
    int a = getchar(); 

    if (a == -15) {
        free(str); 
    }
}