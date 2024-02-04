#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main () {
    // entry
    char *str;
    int a = getchar();

    str = (char *) malloc(15);
    strcpy(str, "helloworld");
    printf("String = %s,  Address = %u\n", str, str);

    free(str);


    // for called methods, if any single one of these branches does not satisfy a must call, then we conclude at the end that must call is not satisfied
    if (a == -15) { // impossible

        // if.then, preds = {entry}
    } else if (a == -10) { // impossible
        // if.then5, preds = {if.else}
        if (a == -5) {

            // if.then7, preds = {if.then5}
        } else if (a == -100) {

            // if.then10, preds = {if.else8}
        } else {
            // if.else11, preds = {if.else8}
            realloc(str, 25);
        }

        // if.end13, preds = {if.end, if.then7}
    } else {
        // if.else14, preds = {if.else}
        free(str);
    }



    // if.end16, preds = {if.end15, if.then}
    return(0);
}
