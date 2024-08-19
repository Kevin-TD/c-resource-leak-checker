#ifndef UTILS_H
#define UTILS_H

#include "RunAnalysis.h"

extern const char *WHITESPACES;

namespace rlc_dataflow {

/**
 * @brief Get a human-readable string name for an llvm Value
 *
 * @param Val The llvm Value to get the string representation of
 * @return std::string The string representation of Val.
 */
std::string variable(const Value *Val);

// unwraps a PointerType until there is a StructType. if there is no
// StructType, NULL is returned.
StructType *unwrapValuePointerToStruct(Value *value);

// checks to see if the name of some struct is present
// in any IR debug !DICompositeType tags. these tags are
// more reliable in preserving the original struct name from
// the C code. this is used because struct names from non-debug info
// in the IR do not have to adhere to any format (though they typically
// look like struct.[C struct name]).
bool IRstructNameEqualsCstructName(std::string &structName,
                                   std::string &optLoadFileName);

// gets the predecessors of a given instruction in the control-flow graph
std::vector<Instruction *> getPredecessors(Instruction *instruction);

// gets the successors of a given instruction in the control-flow graph
std::vector<Instruction *> getSuccessors(Instruction *instruction);

} // namespace rlc_dataflow

namespace rlc_util {

bool isNumber(const std::string &s);

std::vector<std::string> splitString(const std::string &input, char delimiter);

void removeWhitespace(std::string &input);

// inclusive bounds [i, j]
std::string sliceString(const std::string &str, int i, int j);

/*
returns true iff str begins with a letter or an underscore and is followed by
zero or more letters, numbers, or underscores
*/
bool isValidCVariableName(const std::string &str);

bool hasOnlyOneBalancedParentheses(const std::string &str);

bool startsWith(std::string str, std::string starts);

// for debugging; just an easier way to get a string rep of a set
std::string setToString(std::set<std::string> &setString);

// opens file with name filePath and returns the nth line of it
std::string getNthLine(const std::string& filePath, unsigned n);

// for some .c file ../test/<dir>/<file_name>.c, <dir>/<file_name> is returned.
// e.g., ../test/simple_layer_test/layer/test1_again.c ->
// simple_layer_test/layer/test1_again
std::string getTestName(std::string optLoadFileName);

} // namespace rlc_util

#endif // UTILS_H