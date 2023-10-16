#include "Utils.h"
#include "Debug.h"
#include "RunAnalysis.h"

const char *WHITESPACES = " \t\n\r";

namespace dataflow {

/*
Code handles IR vars coming from parameters and explicitly defined vars
*/
std::string variable(const Value *Val) {
  std::string Code;
  raw_string_ostream SS(Code);
  Val->print(SS);

  // checks if Code does not contain '=', in which case, treat it as a parameter
  // to a function
  /*
  Typically, variables are initialized by some alloca instruction. So int
  i; would look like %i = alloca i32. This is at least the case for when we
  define new variables inside the body of a function. The case where an '=' did
  not show up was in this IR snippet:
  TODO: verify if all variable are initialized by some alloca instruction

  ; Function Attrs: noinline nounwind uwtable
  define dso_local void @does_free(i8* %s) #0 !dbg !10 {
  entry:
    %s.addr = alloca i8*, align 8
    store i8* %s, i8** %s.addr, align 8
  Here, at the store instruction, when we try getting the string name of %s, the
  Code looks like i8* %s instead of something like %s = alloca i8*. I am
  assuming this is the case since %s is a parameter and not a newly defined
  variable.
  */
  if (Code.find_first_of('=') == std::string::npos) {
    variable_logout("0.FOR VAL "
                    << *Val << " RETURNING "
                    << sliceString(Code, Code.find(' ') + 1, Code.size() - 1));
    return sliceString(Code, Code.find(' ') + 1, Code.size() - 1);
  }

  Code.erase(0, Code.find_first_not_of(WHITESPACES));
  std::string RetVal = Code.substr(0, Code.find_first_of(WHITESPACES));

  if (RetVal == "ret" || RetVal == "br" || RetVal == "store") {
    variable_logout("1.FOR VAL " << *Val << " RETURNING " << Code);
    return Code;
  }
  if (RetVal == "i1" || RetVal == "i8" || RetVal == "i32" || RetVal == "i64") {
    RetVal = Code;
  }
  variable_logout("2.FOR VAL " << *Val << " RETURNING " << RetVal);
  return RetVal;
}

bool isNumber(const std::string &s) {
  char *endPtr;
  std::strtol(s.c_str(), &endPtr, 10);
  return endPtr != s.c_str() && *endPtr == '\0';
}

std::vector<std::string> splitString(const std::string &input, char delimiter) {
  std::vector<std::string> result;
  std::stringstream ss(input);
  std::string token;

  while (getline(ss, token, delimiter)) {
    result.push_back(token);
  }

  return result;
}

void removeWhitespace(std::string &input) {
  input.erase(
      std::remove_if(input.begin(), input.end(),
                     [](char c) { return std::isspace(c) || c == '\0'; }),
      input.end());
}

std::string sliceString(const std::string &str, int i, int j) {
  return str.substr(i, j - i + 1);
}

bool isValidCVariableName(const std::string &str) {
  if (str.empty() || !std::isalpha(str[0]) && str[0] != '_') {
    return false;
  }

  for (int i = 1; i < str.length(); i++) {
    if (!std::isalnum(str[i]) && str[i] != '_') {
      return false;
    }
  }

  return true;
}

bool hasOnlyOneBalancedParentheses(const std::string &str) {
  std::stack<char> parenthesesStack;
  int balancedPairs = 0;

  for (char c : str) {
    if (c == '(') {
      parenthesesStack.push(c);
    } else if (c == ')') {

      // Unbalanced: encountered closing parenthesis without an opening one
      if (parenthesesStack.empty()) {
        return false;
      }
      parenthesesStack.pop();
      balancedPairs++;
    }
  }

  return parenthesesStack.empty() && balancedPairs == 1;
}

std::string setToString(std::set<std::string> &setString) {
  std::string result = "{";
  int setStringSize = setString.size();

  int iterator = 0;
  for (std::string str : setString) {
    result += str;
    if (iterator != setStringSize - 1) {
      result += ", ";
    }
    iterator++;
  }
  result += "}";
  return result;
}

bool startsWith(std::string str, std::string starts) {
  return str.rfind(starts, 0) == 0;
}

} // namespace dataflow