#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct M {
    char* x MustCall("free, free1");
    char* y MustCall("free");
};

void free1(void* p Calls("free1")) {
    free(p);
}

int main() {
    struct M s;

    s.x = (char*)malloc(15);

    s.x = s.y; // note: when we alias s.y and s.x, the mc of the set in total will be {"free", "free1"}
    // how does this happen? we *first* do the alias reasoning and find out s.x <-> s.y,
    // and then in the dataflow pass, we add the must call annotations
    //  in the allocainst section of transfer

    // another note: the section where we auto assume mc's will be removed soon.
    // this test will definitely change after that change


    free(s.x);

    if (getchar()) {
        free1(s.x);
    } else {
        free1(s.y);
    }
}