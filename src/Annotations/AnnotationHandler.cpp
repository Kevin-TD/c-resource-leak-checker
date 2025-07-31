#include "Annotations/AnnotationHandler.h"

AnnotationHandler::AnnotationHandler() {}

void AnnotationHandler::addAnnotation(const std::string &rawAnnotationString) {
    Annotation *anno = Annotation::generateAnnotation(rawAnnotationString);
    logout("anno string rep: " << anno->toString());

    if (StructAnnotation *structAnno = dynamic_cast<StructAnnotation *>(anno)) {
        this->annotations.push_back(structAnno);
    } else if (FunctionAnnotation *functionAnno =
                   dynamic_cast<FunctionAnnotation *>(anno)) {
        this->annotations.push_back(functionAnno);
    } else if (ParameterAnnotation *paramAnno =
                   dynamic_cast<ParameterAnnotation *>(anno)) {
        this->annotations.push_back(paramAnno);
    } else if (ReturnAnnotation *returnAnno =
                   dynamic_cast<ReturnAnnotation *>(anno)) {
        this->annotations.push_back(returnAnno);
    }
}

void AnnotationHandler::addAnnotations(
    const std::vector<std::string> &rawAnnotationStrings) {
    for (auto annoString : rawAnnotationStrings) {
        this->addAnnotation(annoString);
    }
}

Annotation *
AnnotationHandler::getFunctionAnnotation(const std::string &functionName) const {
    for (Annotation *anno : this->annotations) {
        if (FunctionAnnotation *functionAnno =
                    dynamic_cast<FunctionAnnotation *>(anno)) {
            if (functionAnno->functionNameEquals(functionName)) {
                return functionAnno;
            }
        }
    }
    return new ErrorAnnotation();
}

Annotation *
AnnotationHandler::getParameterAnnotation(const std::string &functionName,
        unsigned parameterIndex) const {
    for (Annotation *anno : this->annotations) {
        if (ParameterAnnotation *paramAnno =
                    dynamic_cast<ParameterAnnotation *>(anno)) {
            if (paramAnno->functionNameEquals(functionName) &&
                    paramAnno->parameterIndexEquals(parameterIndex) &&
                    !paramAnno->hasFieldIndex()) {
                return paramAnno;
            }
        }
    }
    return new ErrorAnnotation();
}

Annotation *
AnnotationHandler::getParameterAnnotation(const std::string &functionName,
        unsigned parameterIndex, unsigned fieldIndex) const {
    for (Annotation *anno : this->annotations) {
        if (ParameterAnnotation *paramAnno =
                    dynamic_cast<ParameterAnnotation *>(anno)) {
            if (paramAnno->functionNameEquals(functionName) &&
                    paramAnno->parameterIndexEquals(parameterIndex) &&
                    paramAnno->fieldIndexEquals(fieldIndex)) {
                return paramAnno;
            }
        }
    }
    return new ErrorAnnotation();
}

std::vector<Annotation *>
AnnotationHandler::getAllParameterAnnotationsWithFields(
    const std::string &functionName) const {
    std::vector<Annotation *> annotationsWithFields;
    for (Annotation *anno : this->annotations) {
        if (ParameterAnnotation *paramAnno =
                    dynamic_cast<ParameterAnnotation *>(anno)) {
            if (paramAnno->functionNameEquals(functionName) &&
                    paramAnno->hasFieldIndex()) {
                annotationsWithFields.push_back(paramAnno);
            }
        }
    }

    return annotationsWithFields;
}

std::vector<Annotation *>
AnnotationHandler::getAllParameterAnnotationsWithoutFields(
    const std::string &functionName) const {
    std::vector<Annotation *> annotationsWithoutFields;
    for (Annotation *anno : this->annotations) {
        if (ParameterAnnotation *paramAnno =
                    dynamic_cast<ParameterAnnotation *>(anno)) {
            if (paramAnno->functionNameEquals(functionName) &&
                    !paramAnno->hasFieldIndex()) {
                annotationsWithoutFields.push_back(paramAnno);
            }
        }
    }

    return annotationsWithoutFields;
}

Annotation *
AnnotationHandler::getReturnAnnotation(const std::string &functionName) const {
    for (Annotation *anno : this->annotations) {
        if (ReturnAnnotation *returnAnno = dynamic_cast<ReturnAnnotation *>(anno)) {
            if (returnAnno->functionNameEquals(functionName) &&
                    !returnAnno->hasFieldIndex()) {
                return returnAnno;
            }
        }
    }
    return new ErrorAnnotation();
}

Annotation *
AnnotationHandler::getReturnAnnotation(const std::string &functionName,
                                       unsigned fieldIndex) const {
    for (Annotation *anno : this->annotations) {
        if (ReturnAnnotation *returnAnno = dynamic_cast<ReturnAnnotation *>(anno)) {
            if (returnAnno->functionNameEquals(functionName) &&
                    returnAnno->fieldIndexEquals(fieldIndex)) {
                return returnAnno;
            }
        }
    }
    return new ErrorAnnotation();
}

Annotation *
AnnotationHandler::getStructAnnotation(const std::string &structName,
                                       unsigned fieldIndex) const {
    for (Annotation *anno : this->annotations) {
        if (StructAnnotation *structAnno = dynamic_cast<StructAnnotation *>(anno)) {
            if (structAnno->structNameEquals(structName) &&
                    structAnno->fieldIndexEquals(fieldIndex)) {
                return structAnno;
            }
        }
    }
    return new ErrorAnnotation();
}