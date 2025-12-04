#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

typedef struct Box {
    long *inside;
} Box;

struct Box *MustCall("free", "inside") MustCall("free") create(Owning() long *item) {
    Box *b = malloc(sizeof(Box));
    b->inside = item;
    return b;
}

int main () {
    long *item = malloc(sizeof(long));
    *item = 1;
    Box *box = create(item);
    free(box->inside);
    free(box);

    return 0;
}
