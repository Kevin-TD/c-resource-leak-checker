#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../Annotations/Annotations.h"

MustCall(FUNCTION("does_malloc") AT_PARAMETER("1"), METHODS("free"))
void does_malloc(char* str) {
    str = (char*)malloc(15); 
}

Calls(FUNCTION("does_free") AT_PARAMETER("1"), METHODS("free"))
void does_free(char* str) {
    free(str); 
}


int main() {
    // malloc 
    char* str; 
    does_malloc(str); 
    does_free(str); 

    return 0; 

}