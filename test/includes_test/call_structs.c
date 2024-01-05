#include "call_structs.h"

// clang -fsyntax-only -Xclang -ast-dump ../test/test23/test23.c
// generates AST for this file (assuming in build dir)

// clang-9 -fsyntax-only -Xclang -ast-dump=json ../test/test23/test23.c > ../test/test23/test23_ast.json
// loads AST into json

// clang -fsyntax-only -Xclang -ast-dump ../test/test23/test23.c > ../test/test23/test23.ast

// clang -fsyntax-only -Xclang -ast-dump -ast-dump-filter=FieldDecl ../test/test23/test23.c

// if annotation comes from diff .h file, AST can tell us where it came from on the same AnnotateAttr node
// otherwise, it just says what line and col from the program it came from

// annotations on fields come from RecordDecl -> FieldDecl -> AnnotateAttr
// annotations on function params come from FunctionDecl -> ParmVarDecl -> AnnotateAttr
// annotations on functions themselves come from FunctionDecl -> AnnotateAttr
// since we are only working with MustCall and Calls for now, we'll assume
// all annotations on functions themselves always refer to its return.

// NOTE: annotating on both .c and .h annotates it twice
// (it'll show that one came from .c and one came from h.)

// annotations can be in either .c or .h. if it's in both, check if they're the same


void does_free(char* s Calls("free")) {
    free(s);
}

char* MustCall("free") creates_obligation(char* s Calls("free"), my_struct X Calls("free", "x")) {
    free(s);
    free(X.x);
    char* str = (char*)malloc(15);
    return str;
}

my_struct Calls("free", "x") does_something(my_struct S Calls("free", "x")) {
    free(S.x);
    return S;
}

int main() {
    // should we have to worry about cases where structs are defined
    // inside a method body? the AST cannot easily extract annotations
    // when the struct is inside a method body (we to go inside the method).

    struct my_struct k;
    my_struct_2 m;
    struct my_struct_3 km;

    k.x = (char*)malloc(15);
    k.y = (char*)malloc(15);
    // obligations to free k.x and k.y created

    m.y = (char*)malloc(15);
    km.x = (char*)malloc(15);

    char* s = (char*)malloc(15);
    char* random_var = creates_obligation(s, k);  // creates obligation to free random_var; the method itself also calls free on k.x

    free(random_var); // random_var, k.x freed. now to free k.y
    does_free(k.y); // k.y freed

    does_free(m.y);
    does_free(km.x);

    // all obligations satisfied

}