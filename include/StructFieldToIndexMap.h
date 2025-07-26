#ifndef STRUCT_FIELD_TO_INDEX_H
#define STRUCT_FIELD_TO_INDEX_H

#include "Utils.h"
#include "TempFileManager.h"

/*
string map between struct var name + struct field to struct name + struct index.
example:

`struct my_struct {`
 `char* x;`
 `char* y;`
`}`

`int main() {`
    `struct my_struct A;`
    `struct my_struct B;`
`}`

for this C code, the map will look like:
`[A.x] => [A.0]`
`[A.y] => [A.1]`
`[B.x] => [B.0]`
`[B.y] => [B.1]`

this is used for writing test cases where we may need to specify the index of some variable.
e.g., we may need to test if M.x and M.y has called free. the user may always reference it by
index (e.g., `M.0`) but this is inconvenient and can be less trivial if the struct has many fields.

LLVM stores fields of structs as indicies, so we cannot directly reference them by name (e.g., `M.x`).
thus, this map translates name references into how LLVM recognizes them.

the AST of a program is used to generate the map.
*/
class StructFieldToIndexMap {
  private:
    std::map<std::string, std::string> fieldToIndexMap;
  public:
    StructFieldToIndexMap();

    // builds the map based off a tmp file which consists of the AST of a C program
    void buildMap(const TempFileManager& astInfoFile);

    // returns the corresponding struct name and index based on structNameAndField
    std::string get(const std::string& structNameAndField);

    // returns true iff structNameAndField has a corresponding struct name and index, and false otherwise.
    bool structNameAndFieldIsInMap(const std::string& structNameAndField);
};

#endif