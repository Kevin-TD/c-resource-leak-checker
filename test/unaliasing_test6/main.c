#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free");
    char* y MustCall("free");
};

void reassignPointer(char* s MustCall("free")) {
    s = (char*)malloc(15); 
}

int main() {
    struct M s;
    char* s1;

    s.x = (char*)malloc(15);

    s.y = s.x;
    s1 = s.x; // s1, s.x, s.y aliased

    printf("%s", s.y); 
    
    reassignPointer(s.y); // s.y no longer aliased

    free(s.x); // s1, s.y freed. s.y not
}