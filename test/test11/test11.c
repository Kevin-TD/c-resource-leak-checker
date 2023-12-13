#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

void* MustCall("free0") malloc0(size_t __size)
{
    return malloc(__size);
}

void* MustCall("free1") malloc1(size_t __size)
{
    return malloc(__size);
}

void* MustCall("free2") malloc2(size_t __size)
{
    return malloc(__size);
}

void* MustCall("free3") malloc3(size_t __size)
{
    return malloc(__size);
}

int free0(void* p Calls("free0"))
{
    free(p);
    return 1;
}

int free1(void* p Calls("free1"))
{
    free(p);
    return 1;
}

int free2(void* p Calls("free2"))
{
    free(p);
    return 1;
}

int free3(void* p Calls("free3"))
{
    free(p);
    return 1;
}



int main ()
{
    char *str;
    int a = getchar();

    str = (char *) malloc(15);

    if (free0(str)) {

    } else {

    }

    if (a == -15) {

    } else if (free1(str)) {

    }

    while (free2(str)) {

    }



    return(0);


}

/*
Results
while.end str {free0, free2}


*/