#include "Utils.h"
#include "RunAnalysis.h"

const char *WHITESPACES = " \t\n\r";
const size_t VARIABLE_PADDED_LEN = 8;

namespace dataflow {

std::string variable(const Value *Val) {
  std::string Code;
  raw_string_ostream SS(Code);
  Val->print(SS);
  Code.erase(0, Code.find_first_not_of(WHITESPACES));
  auto RetVal = Code.substr(0, Code.find_first_of(WHITESPACES));
  if (RetVal == "ret" || RetVal == "br" || RetVal == "store") {
    return Code;
  }
  if (RetVal == "i1" || RetVal == "i8" || RetVal == "i32" || RetVal == "i64") {
    RetVal = Code;
  }
  return RetVal;
}

bool isNumber(const std::string &s) {
  char *endPtr;
  std::strtol(s.c_str(), &endPtr, 10);
  return endPtr != s.c_str() && *endPtr == '\0';
}

} // namespace dataflow