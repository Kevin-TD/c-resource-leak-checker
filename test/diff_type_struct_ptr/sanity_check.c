#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"
#pragma pack(push, 1)

typedef struct PairA {
    int a; 
    int b; 
    char* letters MustCall("free"); 
} PairA;

typedef struct PairB {
    int a; 
    char* letters MustCall("free"); 
} PairB;

int main() {
     printf("sizeof PairA = %lu, sizeof PairB = %lu\n", sizeof(PairA), sizeof(PairB));
    // 16 and 16 without #pragma pack(push, 1)
    // 16 and 12 with #pragma pack (push, 1)

    // pragma pack essentially removes padding done in memory. uses less memory 
    // but padding is done in the first place for performance gains. 
    // info: https://learn.microsoft.com/en-us/cpp/preprocessor/pack?view=msvc-170 

    // after these tests i conclude that we should not try to alias 
    // a pointer struct that has a different type to what it's pointing 
    // too. what happens memory wise is too weird. 
}