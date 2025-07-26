#ifndef STRUCT_FIELD_TO_INDEX_TESTER_H
#define STRUCT_FIELD_TO_INDEX_TESTER_H

#include "Utils.h"
#include "StructFieldToIndexMap.h"

/* tests `StructFieldToIndexTester`. language of verification looks like
- `<struct>.<name:str>=<struct>.<index:uint>`

e.g., for `struct M {x, y}` and `variable M m`, there should be
`m.x=m.0` and `m.y=m.1`
*/
class StructFieldToIndexTester {
  public:
    // returns EXIT_SUCCESS if tests passed and EXIT_FAILURE if failed.
    // testName should simply refer to the name of a folder in the
    // test directory. e.g., "if_lub_test".
    static bool runTest(const std::string& testName, StructFieldToIndexMap& structFieldToIndexMap);
};

#endif