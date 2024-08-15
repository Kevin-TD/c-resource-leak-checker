#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Annotations/Annotations.h"


int main() {
    char* s1;
    char* s2;
    char* s3;

    s1 = (char*)malloc(15);
    s2 = s1; 
    s3 = s2; 

    s2 = (char*)malloc(15);

}