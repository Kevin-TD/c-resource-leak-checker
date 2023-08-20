#include "test23.h"

void does_free(char* s Calls(FUNCTION("does_free") AT_PARAMETER("0"), METHODS("free")), char* p Calls(FUNCTION("does_free") AT_PARAMETER("1"), METHODS("free"))) {
    free(s); 
    free(p); 
}
