#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main () {
    // entry
    char *str = (char *) malloc(15);
    int a = getchar();

    if (a == -15) {
        // if.then, preds = {entry}
        free(str);
    }
// if.end, preds = {if.then, entry}
}