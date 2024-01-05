#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

void* MustCall("free") malloc0(size_t __size) {
    return malloc(__size);
}

void free0(void* p Calls("free")) {
    free(p);
}

int main () {
    char *str;

    str = (char *) malloc0(15);
    free0(str);

    return 0;


}