#include "RunAnalysis.h"

#define DEBUG true
#define UTIL_VARIABLE_FUNC_DEBUG false
#define BUILD_PROGRAM_LINES_BRANCH_INFO false
#define RUN_UTIL_FUNCTION_TESTS true

#if DEBUG
#define logout(x) errs() << x << "\n"
#else
#define logout(x)
#endif

#if UTIL_VARIABLE_FUNC_DEBUG
#define variable_logout(x) errs() << x << "\n"
#else
#define variable_logout(x)
#endif