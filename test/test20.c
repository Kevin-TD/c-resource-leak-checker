#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../Annotations/Annotations.h"

struct my_struct {
    char* x; 
    char* y;
};

void does_free(char* s) { 
    free(s); 
}


char* creates_obligation(char* s, struct my_struct X) {    
    free(s);  
    free(X.x); 
    char* str = (char*)malloc(15); 
    return str; 
}


struct my_struct does_something(struct my_struct S) {
    free(S.x); 
    return S; 
} 

void does_malloc_calls(struct my_struct S) {
    S.x = (char*)malloc(15);
    S.y = (char*)malloc(15); 
}

void does_free_calls(struct my_struct S) {
    free(S.x);
    free(S.y); 
}




/*
Right now I'd like the AT_PARAMETER and AT_FIELD to be 0-indexed 
(I used 1-indexing before since I saw RLC use it with the field=#..., but just to be more consistent with LLVM I'll use 0-indexing) since if we had an annotation like AT_FIELD("x"), 
it would be impossible using just the IR to figure out which field it was actually referring to since the IR shows fields as indices and not names. 
But I imagine the user won't need to specify fields in the future so it should be okay for now
*/
MustCall(STRUCT("my_struct") AT_FIELD("0"), METHODS("free"))
MustCall(STRUCT("my_struct") AT_FIELD("1"), METHODS("free"))
Calls(FUNCTION("does_something") AT_RETURN AT_FIELD("x"), METHODS("free"))
Calls(FUNCTION("creates_obligation") AT_PARAMETER("0"), METHODS("free"))
Calls(FUNCTION("creates_obligation") AT_PARAMETER("1") AT_FIELD("x"), METHODS("free")) 
MustCall(FUNCTION("creates_obligation") AT_RETURN, METHODS("free")) 
Calls(FUNCTION("does_free") AT_PARAMETER("2"), METHODS("free"))
int main() {   
    struct my_struct M; 
    

    // does_malloc_calls(M);
    // does_free_calls(M);
}