#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
    char* str;

    char** p = &str;
    char*** x = &p;

    *p = (char*)malloc(15);

    strcat(*p, "hello");
    printf("String = %s,  Address = %u\n", *p, *p);


    free(**x);
    return(0);


}