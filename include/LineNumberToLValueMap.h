#ifndef LINE_NUMBER_TO_L_VALUE_MAP_H
#define LINE_NUMBER_TO_L_VALUE_MAP_H

#include "Utils.h"
#include "TempFileManager.h"

/*
maps line numbers to l-value names
example program:
(1) a = 1
(2) b = 2
(3) c
(4)    =
(5) 3

our output is then
1=a
2=b
5=c

with the LHS referring to the line number of the l-value
and the RHS referring to the l-value

note: 5=c because it matches what the LLVM IR does in terms
of listing the line number for each instruction
*/
class LineNumberToLValueMap {
  private:
    std::map<unsigned, std::string> lineNumberToLValueMap;
  public:
    LineNumberToLValueMap();

    // builds the map based off a tmp file which consists of the AST info of a C program
    void buildMap(const TempFileManager& astInfoFile);

    // returns the corresponding l-value name based on lineNumber;
    std::string get(unsigned lineNumber);

    // returns true iff lineNumber has a corresponding l-value name, and false otherwise
    bool lineNumberIsInMap(unsigned lineNumber);
};

#endif