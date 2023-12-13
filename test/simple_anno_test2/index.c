#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

void does_malloc(char* str MustCall("free"))
{
    str = (char*)malloc(15);
}

void does_free(char* str Calls("free"))
{
    free(str);
}


int main()
{
    // malloc
    char* str;
    does_malloc(str);
    does_free(str);

    return 0;

}