#include "Annotations/Annotation.h"
#include "Annotations/ErrorAnnotation.h"
#include "Annotations/FunctionAnnotation.h"
#include "Annotations/ParameterAnnotation.h"
#include "Annotations/ReturnAnnotation.h"
#include "Annotations/StructAnnotation.h"
#include "Debug.h"
#include "Utils.h"

bool Annotation::annotationIsVerified() {
    return this->isVerified;
}

AnnotationType Annotation::getAnnotationType() {
    return this->annotationType;
}

std::set<std::string> Annotation::getAnnotationMethods() {
    return this->annotationMethods;
}

std::string Annotation::getName() {
    return this->targetName;
}

// TODO: error reporting should be more robust and not just a debug logout
// message; it should potentially be managed by a class that contains the
// different types of errors

/*
How correct formatting is defined:

For some string s and some char of string s[i], it can either be alphanumeric
(alnum), comma, or space, or we could be at the end of the string (EOS)

if s[i] is alphanumeric:
  -s[i+1] can be alnum, comma, EOS
  -it can't be space
if s[i] is comma:
  -s[i+1] can be space, alnum
  -it can't be EOS, comma
if s[i] is space:
  -s[i+1] can be alnum
  -it can't be space, EOS, comma

Also, the string cannot be empty and s[0] must be alnum

Example valid strings: "x", "x, y", "x,y,z", "x, y,z", "x, y, z"
Example invalid strings: " x", "x y", "x,  y", "x,y,z  "
*/
bool Annotation::methodsArgumentIsCorrectlyFormatted(
    const std::string &rawMethodsString) {
    int rawMethodsStringSize = rawMethodsString.size();

    if (rawMethodsStringSize == 0) {
        logout("Method Args Error 0 on '" << rawMethodsString
               << "': string must not be empty");
        return false;
    }

    if (!std::isalnum(rawMethodsString[0])) {
        logout("Method Args Error 1 on '"
               << rawMethodsString
               << "': first character of string must be alphanumeric");
        return false;
    }

    for (int i = 0; i < rawMethodsStringSize; i++) {
        if (rawMethodsString[i] == ',') {
            if ((i == rawMethodsStringSize - 1) || rawMethodsString[i + 1] == ',') {
                logout("Method Args Error 2 on '"
                       << rawMethodsString
                       << "': comma must not be last character and a comma cannot "
                       "follow a comma");
                return false;
            }
        } else if (rawMethodsString[i] == ' ') {
            if ((i == rawMethodsStringSize - 1) || rawMethodsString[i + 1] == ' ' ||
                    rawMethodsString[i + 1] == ',') {
                logout("Method Args Error 3 on '"
                       << rawMethodsString
                       << "': space must be followed by alnum and cannot be last char, "
                       "another space, or a comma");
                return false;
            }
        } else if (std::isalnum(rawMethodsString[i])) {
            if ((i != rawMethodsStringSize - 1) && rawMethodsString[i + 1] == ' ') {
                logout("Method Args Error 4 on '"
                       << rawMethodsString
                       << "': alnum character cannot be followed by space");
                return false;
            }
        }
    }
    return true;
}

// a raw correct annotation looks like:
// (Calls || MustCall) target = {FUNCTION ||
// STRUCT}(name).?PARAM(int).?FIELD(int) methods = str

// FIELD argument refers to the index that belongs to some struct
// e.g., struct S = {x, y}, S.x is index 0, and S.y is index 1.
// the AST pass converts names to indicies.

// examples:
/*
Calls target = FUNCTION(does_free) methods = free
-- FunctionAnnotation; not anticipated that we'll be needing it but some
handling for it is here just in case

Calls target = FUNCTION(does_free).PARAM(1) methods = free
-- ParameterAnnotation

Calls target = FUNCTION(creates_obligation).PARAM(2).FIELD(0)
methods = free
-- ParameterAnnotation with field

MustCall target = FUNCTION(creates_obligation).RETURN methods =
free
-- ReturnAnnotation

Calls target = FUNCTION(does_something).RETURN.FIELD(0) methods =
free
-- ReturnAnnotation with field

MustCall target = STRUCT(my_struct).FIELD(0) methods = free
-- StructAnnotation; always has FIELD specifier

Example for chunks:

Calls target = FUNCTION(creates_obligation).PARAM(2).FIELD(0)
methods = free1, free2

chunks = ['Calls', 'target', '=',
'FUNCTION(creates_obligation).PARAM(2).FIELD(x)', 'methods', '=', 'free1,',
'free2']

chunks[0] is annotation type (Calls/MustCall)
chunks[1] is "target" keyword
chunks[2] is "="
chunks[3] is the target itself (FUNCTION/STRUCT potentially
with valid PARAM and FIELD specifiers)
chunks[4] is "methods" keyword
chunks[5] is "="
chunks[6..end] are all the methods
*/

// TODO: make method more generic and write automated testing for that
bool Annotation::rawStringIsCorrectlyFormatted(
    const std::string &rawAnnotationString) {
    std::vector<std::string> chunks =
        rlc_util::splitString(rawAnnotationString, ' ');

    if (chunks.size() < 7) {
        logout("Annotation String Error 0: malformed string '"
               << rawAnnotationString
               << "' potentially missing argument or string is not annotation "
               "related at all");
        return false;
    }
    // chunks.size() >= 7

    if (chunks[0] != "Calls" && chunks[0] != "MustCall") {
        logout("Annotation String Error 2: invalid annotation '"
               << rawAnnotationString << "' annotation type '" << chunks[1]
               << "' is unknown; expected Calls or MustCall");
        return false;
    }
    // chunks[0] == "Calls" or chunks[0] == "MustCall"

    if (chunks[1] != "target") {
        logout("Annotation String Error 3: invalid annotation '"
               << rawAnnotationString
               << "' 'target' keyword missing or inappropriately placed; '"
               << chunks[1] << "' found");
        return false;
    }
    // chunks[1] == "target"

    if (chunks[2] != "=") {
        logout("Annotation String Error 4: invalid annotation '"
               << rawAnnotationString
               << "' keyword '=' after keyword 'target' is missing or "
               "inappropriately placed; '"
               << chunks[2] << "' found");
        return false;
    }
    // chunks[2] == "="

    //* --- chunks[3] checker section ---
    std::vector<std::string> targetChunks = rlc_util::splitString(chunks[3], '.');
    // {FUNCTION || STRUCT}(name).?PARAM(int).?FIELD(str) ->
    // ["FUNCTION(name)" or "STRUCT(name)", optional "PARAM(int)", optional
    // "FIELD(str)"] required: 1 <= targetChunks.size() <= 3
    int targetChunksSize = targetChunks.size();

    if (targetChunksSize < 1 || targetChunksSize > 3) {
        logout("Annotation String Error 5: invalid annotation '"
               << rawAnnotationString
               << "' target is malformed; too few or too many '.'s, causing "
               "unexpected target chunk size of "
               << targetChunksSize);
        return false;
    }
    // 1 <= targetChunks.size() <= 3

    // targetChunks[0] should look like FUNCTION(...) or STRUCT(...)
    // should have one (and only one) set of balanced parentheses
    if (!rlc_util::hasOnlyOneBalancedParentheses(targetChunks[0])) {
        logout("Annotation String Error 6: invalid annotation '"
               << rawAnnotationString << "' type FUNCTION or STRUCT specification '"
               << targetChunks[0]
               << "' is malformed; # of balanced parentheses does not equal "
               "one");
        return false;
    }

    // targetChunks[0] has '(' and ')'
    std::string targetType = targetChunks[0].substr(0, targetChunks[0].find('('));

    if (targetType != "FUNCTION" && targetType != "STRUCT") {
        logout("Annotation String Error 7: invalid annotation '"
               << rawAnnotationString
               << "' type FUNCTION or STRUCT specification not found; found '"
               << targetType << "'");
        return false;
    }
    // targetChunks[0] starts with "FUNCTION(" or "STRUCT("

    std::string name =
        rlc_util::sliceString(targetChunks[0], targetChunks[0].find('(') + 1,
                              targetChunks[0].find(')') - 1);
    if (!rlc_util::isValidCVariableName(name)) {
        logout("Annotation String Error 8: invalid annotation '"
               << rawAnnotationString << "' invalid target name; target name '"
               << name << "' is not valid C variable name");
        return false;
    }
    // targetChunks[0] starts with "FUNCTION(valid_text" or "STRUCT(int"

    // checks if last character does not equal ")" (it should)
    if (targetChunks[0].find(')') != targetChunks[0].size() - 1) {
        logout("Annotation String Error 9: invalid annotation '"
               << rawAnnotationString
               << "' target is malformed; last character should be ')' but '"
               << targetChunks[0][targetChunks[0].size() - 1] << "' found");
        return false;
    }
    // targetChunks[0] is "FUNCTION/STRUCT(valid_text)"

    if (targetType == "FUNCTION") {
        // targetChunks looks like one of the following:
        // ["FUNCTION(name)"] or
        // ["FUNCTION(name)", "PARAM(int)"] or
        // ["FUNCTION(name)", "PARAM(int)", "FIELD(int)"]
        // ["FUNCTION(name)", "RETURN"] or
        // ["FUNCTION(name)", "RETURN", "FIELD(int)"]

        // targetChunkSize of 1 does not need to be checked; it is valid at this
        // point

        if (targetChunksSize >= 2) {
            // targetChunks[1] must be PARAM(int)
            if (!rlc_util::hasOnlyOneBalancedParentheses(targetChunks[1]) &&
                    targetChunks[1] != "RETURN") {
                logout("Annotation String Error 10: invalid annotation '"
                       << rawAnnotationString
                       << "' target is malformed; # of balanced parentheses does not "
                       "equal one on a non-return target");
                return false;
            }

            std::string paramSpecifier =
                targetChunks[1].substr(0, targetChunks[1].find('('));
            if (paramSpecifier != "PARAM" && targetChunks[1] != "RETURN") {
                logout("Annotation String Error 11: invalid annotation '"
                       << rawAnnotationString
                       << "' 'PARAM' keyword missing/malformed on non-return target; '"
                       << paramSpecifier << "' found");
                return false;
            }
            // targetChunk[1] is "PARAM(" or "RETURN"

            if (paramSpecifier == "PARAM") {
                std::string nthParameter = rlc_util::sliceString(
                                               targetChunks[1], targetChunks[1].find('(') + 1,
                                               targetChunks[1].find(')') - 1);
                if (!rlc_util::isNumber(nthParameter)) {
                    logout("Annotation String Error 12: invalid annotation '"
                           << rawAnnotationString << "' PARAM argument is not a number; '"
                           << nthParameter << "' found");
                    return false;
                }
                // targetChunks[1] is "PARAM(int"

                if (targetChunks[1].find(')') != targetChunks[1].size() - 1) {
                    logout("Annotation String Error 13: invalid annotation '"
                           << rawAnnotationString
                           << "' target argument is malformed; last character should be "
                           "')' but '"
                           << targetChunks[1][targetChunks[1].size() - 1] << "' found");
                    return false;
                }
                // targetChunks[1] is "PARAM(int)"
            }

            if (targetChunksSize == 3) {
                // targetChunks[2] must be FIELD(int)
                if (!rlc_util::hasOnlyOneBalancedParentheses(targetChunks[2])) {
                    logout("Annotation String Error 14: invalid annotation '"
                           << rawAnnotationString
                           << "' target is malformed; # of balanced parentheses does not "
                           "equal one on '"
                           << targetChunks[2] << "'");
                    return false;
                }

                std::string fieldSpecifier =
                    targetChunks[2].substr(0, targetChunks[2].find('('));
                if (fieldSpecifier != "FIELD") {
                    logout("Annotation String Error 15: invalid annotation '"
                           << rawAnnotationString
                           << "' keyword 'FIELD' missing/malformed; '" << fieldSpecifier
                           << "' found");
                    return false;
                }
                // targetChunk[2] is "FIELD("

                std::string fieldName = rlc_util::sliceString(
                                            targetChunks[2], targetChunks[2].find('(') + 1,
                                            targetChunks[2].find(')') - 1);
                if (!rlc_util::isNumber(fieldName)) {
                    logout("Annotation String Error 16: invalid annotation '"
                           << rawAnnotationString << "' FIELD argument of '" << fieldName
                           << "'is not a number");
                    return false;
                }
                // targetChunks[2] is "FIELD(int"

                if (targetChunks[2].find(')') != targetChunks[2].size() - 1) {
                    logout("Annotation String Error 17: invalid annotation '"
                           << rawAnnotationString
                           << "' target is malformed; last char of '" << targetChunks[2]
                           << "' should be ')'");
                    return false;
                }
                // targetChunks[2] is "FIELD(int)"
            }
        }
    } else if (targetType == "STRUCT") {
        if (targetChunksSize == 1) {
            logout("Annotation String Error 25: invalid annotation '"
                   << rawAnnotationString
                   << "' target is malformed; STRUCT may be missing FIELD specifier "
                   "or specifier is malformed");
            return false;
        }
        // targetChunks looks ["STRUCT(name)", "FIELD(int)"]
        if (!rlc_util::hasOnlyOneBalancedParentheses(targetChunks[1])) {
            logout("Annotation String Error 18: invalid annotation '"
                   << rawAnnotationString
                   << "' target is malformed; number of balanced parentheses on '"
                   << targetChunks[1] << "' does not equal one");
            return false;
        }

        std::string fieldSpecifier =
            targetChunks[1].substr(0, targetChunks[1].find('('));
        if (fieldSpecifier != "FIELD") {
            logout("Annotation String Error 19: invalid annotation '"
                   << rawAnnotationString << "' keyword 'FIELD' missing/malformed, '"
                   << fieldSpecifier << "' found");
            return false;
        }
        // targetChunk[1] is "FIELD("

        std::string fieldName =
            rlc_util::sliceString(targetChunks[1], targetChunks[1].find('(') + 1,
                                  targetChunks[1].find(')') - 1);
        if (!rlc_util::isNumber(fieldName)) {
            logout("Annotation String Error 20: invalid annotation '"
                   << rawAnnotationString << "' FIELD argument '" << fieldName
                   << "' is not a number");
            return false;
        }
        // targetChunks[1] is "FIELD(int"

        if (targetChunks[1].find(')') != targetChunks[1].size() - 1) {
            logout("Annotation String Error 21: invalid annotation '"
                   << rawAnnotationString
                   << "' target is malformed; last character of '" << targetChunks[1]
                   << "' was not ')'");
            return false;
        }
        // targetChunks[1] is "FIELD(int)"
    }
    //* --- chunks[3] checker section close ---
    if (chunks[4] != "methods") {
        logout("Annotation String Error 22: invalid annotation '"
               << rawAnnotationString
               << "' missing keyword 'methods'/malformed methods "
               "argument");
        return false;
    }

    if (chunks[5] != "=") {
        logout("Annotation String Error 23: invalid annotation '"
               << rawAnnotationString << "' malformed methods argument");
        return false;
    }

    // chunk[6..end] should just be method args

    std::string methodsString;
    for (int i = 6; i < chunks.size(); i++) {
        methodsString += chunks[i];

        if (i != chunks.size() - 1) {
            methodsString += " ";
        }
    }

    if (!methodsArgumentIsCorrectlyFormatted(methodsString)) {
        return false;
    }

    rlc_util::removeWhitespace(methodsString);

    std::vector<std::string> methodsVector =
        rlc_util::splitString(methodsString, ',');

    for (std::string &method : methodsVector) {
        if (!rlc_util::isValidCVariableName(method)) {

            logout("Annotation String Error 24: invalid annotation '"
                   << rawAnnotationString << "' method argument '" << method
                   << "' is not valid C string variable name");
            return false;
        }
    }

    if (methodsVector.size() == 0) {
        logout("Annotation String Error 26: invalid annotation '"
               << rawAnnotationString << "' method argument empty");
        return false;
    }

    return true;
}

Annotation *Annotation::generateAnnotation(const std::string &rawAnno) {
    if (!Annotation::rawStringIsCorrectlyFormatted(rawAnno)) {
        return new ErrorAnnotation();
    }

    // we can assume string is correctly formatted

    AnnotationType annoType;
    std::vector<std::string> chunks = rlc_util::splitString(rawAnno, ' ');

    if (chunks[0] == "Calls") {
        annoType = AnnotationType::CallsAnnotation;
    } else if (chunks[0] == "MustCall") {
        annoType = AnnotationType::MustCallAnnotation;
    }

    std::string methodsString;

    // the remaining string is the methods argument
    // e.g., "x", "x, y, z", "x,y", "x, y,z"
    for (int i = 6; i < chunks.size(); i++) {
        methodsString += chunks[i];
    }
    rlc_util::removeWhitespace(methodsString);

    std::vector<std::string> methodsVector =
        rlc_util::splitString(methodsString, ',');
    std::set<std::string> methodsSet(methodsVector.begin(), methodsVector.end());

    std::vector<std::string> targetChunks =
        rlc_util::splitString(chunks[3], '.'); // target = ... section
    int targetChunksSize = targetChunks.size();

    std::string targetType = targetChunks[0].substr(
                                 0, targetChunks[0].find('(')); // equals FUNCTION(...) or STRUCT(...)
    std::string name =
        rlc_util::sliceString(targetChunks[0], targetChunks[0].find('(') + 1,
                              targetChunks[0].find(')') - 1);

    if (targetChunksSize == 1 && targetType == "FUNCTION") {
        return new FunctionAnnotation(annoType, methodsSet, name);
    }

    // all structs targets specify a field
    if (targetChunksSize == 2 && targetType == "STRUCT") {
        int field = std::stoi(rlc_util::sliceString(targetChunks[1],
                              targetChunks[1].find('(') + 1,
                              targetChunks[1].find(')') - 1));
        return new StructAnnotation(annoType, methodsSet, name, field);
    }

    // target is now either ParameterAnnotation or ReturnAnnotation
    // target = FUNCTION(...).RETURN | FUNCTION(...).RETURN.FIELD(...)
    // or FUNCTION(...).PARAM(int) or FUNCTION(...).PARAM(int).FIELD(...)

    if (targetChunksSize >= 2 && targetType == "FUNCTION") {
        std::string targetField = targetChunks[1]; // RETURN or PARAM(int)

        if (targetField == "RETURN") {
            std::string returnField;

            // there is a return field
            if (targetChunksSize == 3) {
                returnField = rlc_util::sliceString(targetChunks[2],
                                                    targetChunks[2].find('(') + 1,
                                                    targetChunks[2].find(')') - 1);
            }

            int returnFieldInt;

            if (returnField.compare("") == 0) {
                returnFieldInt = -1;
            } else {
                returnFieldInt = std::stoi(returnField);
            }

            return new ReturnAnnotation(annoType, methodsSet, name, returnFieldInt);
        }

        // targetField is PARAM(int)
        int nthParameter = std::stoi(rlc_util::sliceString(
                                         targetField, targetField.find('(') + 1, targetField.find(')') - 1));
        std::string parameterField;

        // there is a parameter field
        if (targetChunksSize == 3) {
            parameterField =
                rlc_util::sliceString(targetChunks[2], targetChunks[2].find('(') + 1,
                                      targetChunks[2].find(')') - 1);
        }

        int parameterFieldInt;

        if (parameterField.compare("") == 0) {
            parameterFieldInt = -1;
        } else {
            parameterFieldInt = std::stoi(parameterField);
        }

        return new ParameterAnnotation(annoType, methodsSet, name,
                                       parameterFieldInt, nthParameter);
    }

    return new ErrorAnnotation();
}

namespace rlc_annotation_util {

std::string annotationTypeToString(AnnotationType anno) {
    switch (anno) {
    case AnnotationType::MustCallAnnotation:
        return "MustCallAnnotation";
        break;
    case AnnotationType::CallsAnnotation:
        return "CallsAnnotation";
        break;
    }
    return "";
}

} // namespace rlc_annotation_util