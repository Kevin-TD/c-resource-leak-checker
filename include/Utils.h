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

/**
 * @brief Encode the memory address of an llvm Value
 *
 * @param Val The llvm Value to get the encoding of
 * @return std::string The encoded memory address of Val
 */
std::string address(const Value *Val);

bool isNumber(const std::string& s);


} // namespace dataflow

#endif // UTILS_H