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
        free(str);
        // if.else, preds = {entry}
    } else if (a == -10) { // impossible
        // if.then8, preds = {if.else}
        char* str1;
        str1 = (char *) malloc(15);

        if (a == -9) { // very impossible
            // if.then11, preds = {if.then8}
            free(str1);
        }

        // if.end, preds = {if.then11, if.then8}
        free(str);

    } else {
        // if.else12, preds = {if.else}
        char* str2;
        str2 = (char *) malloc(15);

        if (a == -9) { // very impossible
            // if.then15, preds = {if.else12}
            free(str2);
        }

        // if.end16, preds = {if.then15, if.else12}
        free(str2);
        free(str);
    }


    // if.end18, preds = {if.end17, if.then}
    return(0);
}
