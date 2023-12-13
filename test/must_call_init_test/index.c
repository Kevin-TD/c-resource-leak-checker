#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"

struct my_struct {
    char* x MustCall("free");
    char* y MustCall("free");
};

int main() {
    struct my_struct M;
}