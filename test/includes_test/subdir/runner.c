#include "../call_structs.h"

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