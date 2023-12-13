#include "Annotations/ReturnAnnotation.h"

ReturnAnnotation::ReturnAnnotation(AnnotationType annotationType,
                                   std::set<std::string> annotationMethods,
                                   std::string targetName, int field)
{
    this->annotationType = annotationType;
    this->annotationMethods = annotationMethods;
    this->targetName = targetName;
    this->field = field;
    this->isVerified = false;
}

std::string ReturnAnnotation::generateStringRep()
{
    std::string annoTypeString =
        rlc_annotation_util::annotationTypeToString(this->annotationType);
    std::string annoMethodsString =
        rlc_util::setToString(this->annotationMethods);

    std::string fieldString;
    if (this->field != -1) {
        fieldString = "Field = " + std::to_string(this->field);
    }

    return "@" + annoTypeString +
           " ReturnAnnotation Function Name = " + this->targetName + " " +
           fieldString + " methods = " + annoMethodsString;
}

bool ReturnAnnotation::fieldEquals(int field)
{
    return field == this->field;
}

bool ReturnAnnotation::hasField()
{
    return this->field != -1;
}

bool ReturnAnnotation::functionNameEquals(const std::string &functionName)
{
    return functionName.compare(this->targetName) == 0;
}

int ReturnAnnotation::getField()
{
    return this->field;
}