#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

struct s {
    int *a;
    int *b;
};

int main () {
    struct s* item = malloc(sizeof(struct s));
    item->a = malloc(sizeof(int));
    item->b = malloc(sizeof(int));
    free(item->a);
    free(item);
    free(item->b);
    return 0;
}
