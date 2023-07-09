#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// "calls" annotation, "must call" annotations 

#define Annotate(x) __attribute__((annotate(x)))

#define MustCall(x) __attribute__((annotate("MustCall: {" #x "}")))
#define CalledMethods(x) __attribute__((annotate("CalledMethods: {" #x "}")))
#define MustCallUnkown __attribute__((annotate("MustCallUnknown: {}")))

// unsure how much we'll be needing these 
#define Owning __attribute__((annotate("Owning")))
#define MustCallAlias __attribute__((annotate("MustCallAlias"))) // implementation will be tricky  since this needs a pair of annotations, one on the return type and one for a parameter

// we need to be able to give annotations for an entire function or a paramter, right? 

// note: annotations for parameters are inside the method via a call to `call void @llvm.var.annotation`
// annotations for functions are in @llvm.global.annotations


MustCall("mallocX")
MustCall("mallocY")
void* malloc0(size_t __size) {
    return malloc(__size);
}

MustCall("mallocZ")
void* malloc1(size_t __size MustCall("mallocY")) {
    return malloc(__size);
}

void* malloc2(size_t __size) {
    return malloc(__size);
}

void* malloc3(size_t __size) {
    return malloc(__size);
}

int free0(void* p) {
    free(p);
    return 1; 
}

int free1(void* p) {
    int a = getchar(); 

    if (a == -15) {
        free(p);
    }

    return 1;
}

int free2(void* p) {
    free(p);
    return 1;
}

int free3(void* p) {
    free(p);
    return 1; 
}

void hidden(char* str) {
    int a = getchar(); 

    if (a == -15) {
        str = (char *)malloc(15); 
    }
    else if (a == -10) {
        free(str); 
    }
}



int main () {
   char* str;
   int a = getchar(); 

   if (a == -15) {
    str = (char *) malloc(15);
   }

   else if (a == -10) {
    str = (char *) malloc2(15);
   }
   else {
    str = (char *) malloc1(15);
   }

   hidden(str); 




   return(0);

 
}