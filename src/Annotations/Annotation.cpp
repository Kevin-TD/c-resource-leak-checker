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

/* raw annotations have the (pseduo) regex pattern of
(Calls | MustCall)
target = (
    (FUNCTION(<var name>)
        (.PARAM(<int>) | .RETURN))?
        .FIELD(<int>)?)
    | (STRUCT(<name>)).FIELD(<int>)
)
methods = <var name list>

where '|' is xor and '?' refers to an optional extension

examples:
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
*/
bool Annotation::rawStringIsCorrectlyFormatted(const std::string &rawAnnotationString) {
    std::string callsAnnoRe = "Calls";
    std::string mustCallAnnoRe = "MustCall";
    std::string targetRe = "target";
    std::string structRe = "STRUCT";
    std::string functionRe = "FUNCTION";
    std::string extParamRe = "PARAM";
    std::string extFunctionRe = "FUNCTION";
    std::string extReturnRe = "RETURN";
    std::string extFieldRe = "FIELD";
    std::string methodsRe = "methods";

    std::string varNameRe = "([_A-Za-z])([_A-Za-z0-9])*";
    std::string methodArgRe = "(" + varNameRe + ")(\ ?,\ ?(" + varNameRe + "))*";
    std::string annoNameRe = callsAnnoRe + "|" + mustCallAnnoRe;

    std::regex annoStrRe(
        "(" + annoNameRe + ") " + targetRe + " = (" + structRe + "\\((" + varNameRe + ")\\)|" + functionRe + "\\((" + varNameRe + ")\\)(\\." + extParamRe + "\\([0-9]+\\)|\\." + extReturnRe + ")?)(\\." + extFieldRe + "\\([0-9]+\\))? " + methodsRe + " = (" + methodArgRe + ")");

    return std::regex_match(rawAnnotationString.c_str(), annoStrRe);
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