#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
    // entry
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

        // if.then, preds = {entry}
    } else if (a == -10) { // impossible
        // if.then8, preds = {if.else}
        char* str1;
        str1 = (char *) malloc(15);

        if (a == -9) { // very impossible
            // if.then11, preds = {if.then8}
            free(str1);
        } else {
            // if.else12, preds = {if.then8}
            free(str1);
            realloc(str, 15);
        }

        // if.end, preds = {if.else12, if.then11}
    } else {
        // if.else14, preds = {if.else}
        free(str);
    }


    // if.end16, preds = {if.end15, if.then}
    return(0);
}

