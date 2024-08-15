#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free");
    char* y MustCall("free");
};

int main() {
    struct M s; 
    char* s1; 

    s.x = (char*)malloc(15);

    s.y = s.x; // aliased
    s1 = s.x; // aliased

    s1 = (char*)malloc(15); // s.1 no longer aliased with s.x and s.y

    free(s.x); // s.x and s.y freed, s1 not freed
}