#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

int main () {
    long *item = malloc(sizeof(long));
    if(item)
        free(item);
    return 0;
}
