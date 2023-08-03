#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../Annotations/Annotations.h"

struct my_struct {
    MustCall(STRUCT("my_struct") AT_FIELD("x"), METHODS("free"))
    MustCall(STRUCT("my_struct") AT_FIELD("y"), METHODS("free"))
    char* x; // MustCall("free")
    char* y; // MustCall("free")
};

Calls(FUNCTION("does_free") AT_PARAMETER("1"), METHODS("free"))
void does_free(char* s) { 
    free(s); 
}

/*

Function Annotations are not actually supported but it's there for our consideration 

StructAnnotation, FunctionAnnotation, ParameterAnnotation, ReturnAnnotation 
- AnnotationType [All]: enum AnnotationType {MustCall, Calls}
- Methods [All]: set<str> 
- Name [All]
- Field [Struct, Parameter, Return]: str
- NthParameter (1-indexed) [Parameter]: int

- getAnnotation(struct name = "...", field = "..."): pair<AnnotationType, Methods>
- getAnnotation(function name = "..."): pair<AnnotationType, Methods>
- getAnnotation(function name = "...", nthParameter = ... ):  pair<AnnotationType, Methods> 

- Annotation(raw_annotation)


String: TOOL_CHECKER Calls target = FUNCTION(does_free).PARAM(1) methods = {free}
String: TOOL_CHECKER MustCall target = STRUCT(my_struct).FIELD(x) methods = {free}
String: TOOL_CHECKER MustCall target = STRUCT(my_struct).FIELD(y) methods = {free}
String: TOOL_CHECKER Calls target = FUNCTION(creates_obligation).PARAM(1) methods = {free}
String: TOOL_CHECKER Calls target = FUNCTION(creates_obligation).PARAM(2).FIELD(x) methods = {free}
String: TOOL_CHECKER MustCall target = FUNCTION(creates_obligation).RETURN methods = {free}
String: TOOL_CHECKER Calls target = FUNCTION(does_something).RETURN.FIELD(x) methods = {free}


*/


/*
void does_free(char* s MustCall("free")) { 
    free(s); 
}
*/


Calls(FUNCTION("creates_obligation") AT_PARAMETER("1"), METHODS("free"))
Calls(FUNCTION("creates_obligation") AT_PARAMETER("2") AT_FIELD("x"), METHODS("free"))
MustCall(FUNCTION("creates_obligation") AT_RETURN, METHODS("free")) 
char* creates_obligation(char* s, struct my_struct X) {    
    free(s);  
    free(X.x); 
    char* str = (char*)malloc(15); 
    return str; 
}

/*
char* MustCall("free") creates_obligation(char* s Calls("free"), struct my_struct X Calls("free", "x")) {    
    free(s);  
    free(X.x); 
    char* str = (char*)malloc(15); 
    return str; 
}
*/

Calls(FUNCTION("does_something") AT_RETURN AT_FIELD("x"), METHODS("free"))
struct my_struct does_something(struct my_struct S) {
    free(S.x); 
    return S; 
}

/*
struct my_struct Calls("free", "x") does_something(struct my_struct S Calls("free", "x")) {
    free(S.x); 
    return S; 
}
*/

int main() {
    struct my_struct k; 
    k.x = (char*)malloc(15); 
    k.y = (char*)malloc(15); 
    // obligations to free k.x and k.y created 

    char* s = (char*)malloc(15);  
    char* y = creates_obligation(s, k);  // creates obligation to free y; the method itself also calls free on k.x 

    free(y); // y, k.x freed. now to free k.y 
    does_free(k.y); // k.y freed 
    
    // all obligations satisfied 

}

/*
Annotation output for this test 

String: TOOL_CHECKER Calls target = FUNCTION(does_free).PARAM(1) methods = free  <-- Raw String 
@Calls ParameterAnnotation Function Name = does_free Parameter = #1  methods = {free, } <-- String Representation of Annotation in the code 

String: ../test/test16.c
undefined annotation

String: TOOL_CHECKER MustCall target = STRUCT(my_struct).FIELD(x) methods = free
@MustCall StructAnnotation Struct Name = my_struct Field = x methods = {free, }

String: TOOL_CHECKER MustCall target = STRUCT(my_struct).FIELD(y) methods = free
@MustCall StructAnnotation Struct Name = my_struct Field = y methods = {free, }

String: TOOL_CHECKER Calls target = FUNCTION(creates_obligation).PARAM(1) methods = free
@Calls ParameterAnnotation Function Name = creates_obligation Parameter = #1  methods = {free, }

String: TOOL_CHECKER Calls target = FUNCTION(creates_obligation).PARAM(2).FIELD(x) methods = free
@Calls ParameterAnnotation Function Name = creates_obligation Parameter = #2 Field = x methods = {free, }

String: TOOL_CHECKER MustCall target = FUNCTION(creates_obligation).RETURN methods = free
@MustCall ReturnAnnotation Function Name = creates_obligation  methods = {free, }

String: TOOL_CHECKER Calls target = FUNCTION(does_something).RETURN.FIELD(x) methods = free
@Calls ReturnAnnotation Function Name = does_something Field = x methods = {free, }

*/