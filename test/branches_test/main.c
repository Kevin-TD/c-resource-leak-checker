#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void mainFunc() {
    char *str = (char *) malloc(15);
    int a = getchar();

    if (a == -15) {
        free(str);
    } else if (a == -10) {
        free(str);
    } else {
        free(str);
    }
}

int main() {
    char *str = (char *) malloc(15);
    int a = getchar();

    if (a == -15) {
        free(str);
    }
}