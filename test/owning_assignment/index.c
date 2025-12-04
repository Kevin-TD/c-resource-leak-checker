#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

typedef struct Box {
    long *inside;
} Box;

int MustCall("free", "inside") foo(Box *b) {
    long *item = malloc(sizeof(long));
    *item = 1;
    b->inside = item;
}

int main () {
    Box *box = malloc(sizeof(Box));
    foo(box);
    free(box->inside);
    free(box);
    return 0;
}
