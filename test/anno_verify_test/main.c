#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Annotations/Annotations.h"

// function return case 

char* MustCall("free") foo() { // should be no warning 
    char* x = (char*)malloc(15);
    if (1 == 2) {
      return x;
    } else {
      return x;
    }
}

char* foo_causes_warning() { // should warn that annotation missing on return that is MustCall("free") or "greater" (like, more methods)
// the must call on return should contain method "free"

    char* x = (char*)malloc(15);
    if (1 == 2) {
      return x;
    } else {
      return x;
    }
}

// structs will be a little more complicated to handle 

typedef struct M {
  char* x MustCall("free");
  char* y MustCall("free"); 
} M; 

M MustCall("free", "x") MustCall("free", "y") m_init() {
  M m; 
  return m; 
}

char* MustCall("free") m_init2() {
  M m; 
  return m.x; 
}