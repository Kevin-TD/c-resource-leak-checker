#include "RunAnalysis.h"

#define DEBUG true
#if DEBUG
#define logout(x) errs() << x << "\n";
#else
#define logout(x)
#endif