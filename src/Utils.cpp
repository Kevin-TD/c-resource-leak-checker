#include "Utils.h"
#include "Debug.h"
#include "RunAnalysis.h"

const char *WHITESPACES = " \t\n\r";

namespace rlc_dataflow {

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
        return rlc_util::sliceString(Code, Code.find(' ') + 1, Code.size() - 1);
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

bool IRstructNameEqualsCstructName(std::string &structName,
                                   std::string &optLoadFileName) {
    LLVMContext context;
    SMDiagnostic error;

    std::string IRFileName =
        rlc_util::sliceString(optLoadFileName, 0, optLoadFileName.size() - 3) +
        ".ll";

    std::unique_ptr<Module> module = parseIRFile(IRFileName, error, context);
    if (!module) {
        errs() << "Error: IR file '" + IRFileName + "' not found\n";
        exit(1);
    }

    DebugInfoFinder debugInfoFinder;
    debugInfoFinder.processModule(*module);

    for (auto &DICompositeType : debugInfoFinder.types()) {
        if (DICompositeType->getTag() == dwarf::DW_TAG_structure_type &&
                DICompositeType->getName() == structName) {
            return true;
        }
    }

    return false;
}

std::vector<Instruction *> getPredecessors(Instruction *instruction) {
    std::vector<Instruction *> Ret;
    auto Block = instruction->getParent();
    for (auto Iter = Block->rbegin(), End = Block->rend(); Iter != End; ++Iter) {
        if (&(*Iter) == instruction) {
            ++Iter;
            if (Iter != End) {
                Ret.push_back(&(*Iter));
                return Ret;
            }
            for (auto Pre = pred_begin(Block), BE = pred_end(Block); Pre != BE;
                    ++Pre) {
                Ret.push_back(&(*((*Pre)->rbegin())));
            }
            return Ret;
        }
    }
    return Ret;
}


std::vector<Instruction *> getSuccessors(Instruction *instruction) {
    std::vector<Instruction *> Ret;
    auto Block = instruction->getParent();
    for (auto Iter = Block->begin(), End = Block->end(); Iter != End; ++Iter) {
        if (&(*Iter) == instruction) {
            ++Iter;
            if (Iter != End) {
                Ret.push_back(&(*Iter));
                return Ret;
            }
            for (auto Succ = succ_begin(Block), BS = succ_end(Block); Succ != BS;
                    ++Succ) {
                Ret.push_back(&(*((*Succ)->begin())));
            }
            return Ret;
        }
    }
    return Ret;
}

StructType *unwrapValuePointerToStruct(Value *value) {
    PointerType *valuePointer = dyn_cast<PointerType>(value->getType());
    while (valuePointer) {
        if (StructType *structType =
                    dyn_cast<StructType>(valuePointer->getElementType())) {
            return structType;
        }

        valuePointer = dyn_cast<PointerType>(valuePointer->getElementType());
    }
    return NULL;
}

int getStructNumberOfFields(const std::string& optLoadFileName, const std::string& structTypeName) {
    LLVMContext context;
    SMDiagnostic error;
    std::string IRFileName = rlc_util::sliceString(optLoadFileName, 0, optLoadFileName.size() - 3) + ".ll";
    std::unique_ptr<Module> module = parseIRFile(IRFileName, error, context);

    for (auto structType : module->getIdentifiedStructTypes()) {
        // ASSUMPTION: llvm struct names being with "struct."

        if (
            "struct." + structTypeName == structType->getName()  ||

            rlc_util::startsWith(structTypeName, "struct ") &&
            "struct." + rlc_util::splitString(structTypeName, ' ')[1] == structType->getName()

        ) {
            return structType->getNumElements();
        }
    }

    return -1;
}

std::vector<std::string> getFunctionArgs(const std::string& optLoadFileName, CallInst* call) {
    const DebugLoc &debugLoc = call->getDebugLoc();
    std::string line = rlc_util::getNthLine(optLoadFileName, debugLoc.getLine());
    rlc_util::removeWhitespace(line);
    line = rlc_util::sliceString(line, line.find_first_of('(') + 1, line.find_first_of(')') - 1);
    return rlc_util::splitString(line, ',');
}

} // namespace rlc_dataflow

namespace rlc_util {

bool isNumber(const std::string &s) {
    char *endPtr;
    std::strtol(s.c_str(), &endPtr, 10);

    // checks if it's integer
    if (endPtr != s.c_str() && *endPtr == '\0') {
        return true;
    }

    // if it's not an integer, check if it's a float
    std::istringstream iss(s);
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
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
    [](char c) {
        return std::isspace(c) || c == '\0';
    }),
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

std::string getNthLine(const std::string& filePath, unsigned n) {
    std::ifstream file(filePath);
    std::string line;

    if (!file.is_open()) {
        logout("ERROR: Could not open file " << filePath);
        std::exit(EXIT_FAILURE);
    }

    for (unsigned i = 1; i <= n; ++i) {
        if (!std::getline(file, line)) {
            logout ("ERROR: Line number out of range");
            std::exit(EXIT_FAILURE);
        }
    }

    return line;
}

std::string getTestName(std::string optLoadFileName) {
    std::string startsWith = "../test";
    std::string endsWith = ".c";
    optLoadFileName.replace(0, startsWith.length() + 1, "");
    optLoadFileName.erase(optLoadFileName.length() - 2);

    logout("RES = " << optLoadFileName);

    return optLoadFileName;
}

std::set<std::string> getSymmetricDifference(std::set<std::string> set1, std::set<std::string> set2) {
    std::set<std::string> setDifference;

    std::set_symmetric_difference(set1.begin(), set1.end(),
                                  set2.begin(), set2.end(),
                                  std::inserter(setDifference, setDifference.begin()));

    return setDifference;
}

std::string formatSet(std::string formatSpecifier, std::set<std::string> setToFormat) {
    std::string formattedString;

    for (auto element : setToFormat) {
        std::string replacedString = formatSpecifier.replace(
                                         formatSpecifier.find("{}"),
                                         2,
                                         element
                                     );

        formattedString += replacedString;
    }

    return formattedString;
}

} // namespace rlc_util
