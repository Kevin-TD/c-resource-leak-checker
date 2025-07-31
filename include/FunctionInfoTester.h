#ifndef FUNCTION_INFO_TESTER_H
#define FUNCTION_INFO_TESTER_H

#include "FunctionInfosManager.h"

/* tests `FunctionInfosManager`. the language of verification looks like:
- `function=<str>|return=<str>|params=<set<str>>`
the args can be written in any order but should be on one line.
e.g., `function=malloc0|return=void*|params={size_t,int}`.
if the parameter or return type is a struct,
it must be specified with "struct <struct_name>"
(because the function info pass may return some struct M as either
"M" or "struct M")
*/
class FunctionInfoTester {
  public:
    // returns EXIT_SUCCESS if tests passed and EXIT_FAILURE if failed
    static bool runTest(const std::string& testName, FunctionInfosManager& functionInfosManager);
};

#endif