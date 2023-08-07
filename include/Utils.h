#ifndef UTILS_H
#define UTILS_H

#include "RunAnalysis.h"

extern const char *WHITESPACES;

namespace dataflow {

/**
 * @brief Get a human-readable string name for an llvm Value
 *
 * @param Val The llvm Value to get the string representation of
 * @return std::string The string representation of Val.
 */
std::string variable(const Value *Val);

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

} // namespace dataflow

#endif // UTILS_H