#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MustCall(target, method)                                               \
  __attribute__((annotate("TOOL_CHECKER MustCall " target " " method)))
#define Calls(target, methods)                                                 \
  __attribute__((annotate("TOOL_CHECKER Calls " target " " methods)))

#define METHODS(m) "methods = " m

#define FUNCTION(functionName) "target = FUNCTION(" functionName ")"
#define AT_PARAMETER(nthParameter) ".PARAM(" nthParameter ")"
#define AT_RETURN ".RETURN"
#define STRUCT(structName) "target = STRUCT(" structName ")"

#define AT_FIELD(target) ".FIELD(" target ")"

struct my_struct {
    MustCall(STRUCT("STRUCT(my_struct).x") AT_FIELD("x"), METHODS("free_INVALID")) //* invalid struct name; keyword collision attempt 
    MustCall(STRUCT("target = STRUCT(my_struct).x") AT_FIELD("x"), METHODS("free_INVALID")) //* invalid struct name; keyword collision attempt 
    MustCall(STRUCT("STRUCT(my_struct).x methods = free") AT_FIELD("x"), METHODS("free_INVALID")) //* invalid struct name; keyword collision attempt 
    MustCall(AT_FIELD("x") STRUCT("my_struct"), METHODS("free_INVALID")) // incorrect order; 1st arg must start with STRUCT or FUNCTION
    MustCall(STRUCT("my_struct") AT_RETURN, METHODS("free_INVALID")) // AT_RETURN does not apply to STRUCT
    MustCall(STRUCT("my_struct") AT_PARAMETER("1"), METHODS("free_INVALID")) // AT_PARAMETER does not apply to STRUCT 
    MustCall(STRUCT("my_struct") AT_FIELD("1asd"), METHODS("free_INVALID")) // invalid field name
    MustCall(STRUCT("my_struct"), METHODS("free_INVALID")) // missing FIELD specifier 
    MustCall(STRUCT("my _struct") AT_FIELD("x"), METHODS("free_INVALID")) // invalid struct name 
    MustCall(STRUCT("my_struct") AT_FIELD("x"), METHODS("")) // invalid; empty methods
    MustCall(STRUCT("my_struct") AT_FIELD("x"), METHODS("          ")) // invalid; empty methods
    MustCall(STRUCT("my_struct") AT_FIELD("y"), METHODS("x__INVALID,")) // invalid annotation
    MustCall(STRUCT("my_struct") AT_FIELD("y"), METHODS("x,y,  h")) // invalid annotation
    MustCall(STRUCT("my_struct") AT_FIELD("x"), METHODS("free__VALID")) // valid annotation
    MustCall(STRUCT("my_struct") AT_FIELD("y"), METHODS("free__VALID")) // valid annotation
    MustCall(STRUCT("my_struct") AT_FIELD("y"), METHODS("x, y")) // valid annotation
    MustCall(STRUCT("my_struct") AT_FIELD("y"), METHODS("x,y")) // valid annotation
    MustCall(STRUCT("my_struct") AT_FIELD("y"), METHODS("x,y, z")) // valid annotation
    char* x; // MustCall("free")
    char* y; // MustCall("free")
};

Calls(FUNCTION("does_free") AT_PARAMETER("1"), METHODS("free__VALID")) // valid annotation
void does_free(char* s) { 
    free(s); 
}



Calls(FUNCTION("crea tes_obligation") AT_PARAMETER("1"), METHODS("free_INVALID"))  // invalid function name
Calls(FUNCTION("$creates_obligation") AT_PARAMETER("1"), METHODS("free_INVALID")) // invalid function name
Calls(FUNCTION("creates_obligation") AT_PARAMETER(" "), METHODS("free_INVALID")) // invalid parameter; must be convertible to int
Calls(FUNCTION("creates_obligation") AT_PARAMETER("x"), METHODS("free_INVALID")) // invalid parameter; must be convertible to int
Calls(FUNCTION("creates_obligation") AT_PARAMETER("1"), METHODS("fr ee_INVALID")) // invalid methods
Calls(FUNCTION("creates_obligation") AT_PARAMETER("1"), METHODS("fr e e_INVALID")) // invalid methods
Calls(FUNCTION("creates_obligation") AT_PARAMETER("1"), METHODS("fr e e, fr_INVALID")) // invalid methods
Calls(FUNCTION("creates_obligation") AT_PARAMETER("1"), METHODS("fr e e, fr xr_INVALID")) // invalid methods
Calls(FUNCTION("creates_obligation") AT_PARAMETER("x"), METHODS("fr e e, fr xr_INVALID")) // invalid parameter, invalid methods
Calls(FUNCTION("creates_o bligation") AT_PARAMETER("x"), METHODS("fr e e, fr xr_INVALID")) // invalid function, invalid parameter, invalid methods
MustCall(FUNCTION("creates_obligation") AT_RETURN AT_PARAMETER("1"), METHODS("free_INVALID")) // invalid function; there cannot be both AT_RETURN and AT_PARAMETER
MustCall(FUNCTION("creates_obligation") AT_RETURN METHODS("free"), METHODS("free_INVALID")) // invalid function; there cannot be both AT_RETURN and METHODS
MustCall(FUNCTION("creates_obligation") AT_RETURN AT_RETURN, METHODS("free_INVALID")) // invalid function; there cannot be more than 1 AT_RETURNS 
MustCall("FUNCTION(creates_obligation) RETURN", METHODS("free__INVALID_CHECKING")) // invalid annotation 
Calls(FUNCTION("creates_obligation") AT_PARAMETER("2") AT_FIELD("x"), METHODS("free__VALID")) // valid annotation 
MustCall(FUNCTION("creates_obligation") AT_RETURN, METHODS("free__VALID")) // valid annotation 
char* creates_obligation(char* s, struct my_struct X) {    
    free(s);  
    free(X.x); 
    char* str = (char*)malloc(15); 
    return str; 
}


Calls(FUNCTION("does_something") AT_FIELD("x") AT_RETURN, METHODS("free_INVALID")) // invalid annotation; incorrect order of field and return 
Calls(FUNCTION("does_something") AT_RETURN AT_FIELD("x"), METHODS("free__VALID")) // valid annotation
struct my_struct does_something(struct my_struct S) {
    free(S.x); 
    return S; 
}


int main() {
    struct my_struct k; 
    k.x = (char*)malloc(15); 
    k.y = (char*)malloc(15); 

    char* s = (char*)malloc(15);  
    char* y = creates_obligation(s, k);  

    free(y); 
    does_free(k.y); 

}

