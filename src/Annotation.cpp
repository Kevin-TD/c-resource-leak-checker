#include "Annotation.h"
#include "Debug.h"
#include "FunctionAnnotation.h"
#include "ParameterAnnotation.h"
#include "ReturnAnnotation.h"
#include "StructAnnotation.h"
#include "Utils.h"

Annotation::Annotation() {
  this->annotationType = AnnotationType::UndefinedAnnotation;
}

bool Annotation::annotationIsUndefined() {
  return this->annotationType == AnnotationType::UndefinedAnnotation;
}

Annotation *generateAnnotation(const std::string &rawAnnotationString) {
  AnnotationType annoType = AnnotationType::UndefinedAnnotation;
  std::vector<std::string> chunks =
      dataflow::splitString(rawAnnotationString, ' ');

  FunctionAnnotation *UndefinedAnnotation =
      new FunctionAnnotation(annoType, {}, "");

  // raw annotation
  // TOOL_CHECKER (Calls || MustCall) target = {FUNCTION ||
  // STRUCT}(name).?PARAM(int).?FIELD(str) methods = str examples:
  /*
  TOOL_CHECKER Calls target = FUNCTION(does_free) methods = free
  -- FunctionAnnotation; not anticipated that we'll be needing it but some
  handling for it is here just in case

  TOOL_CHECKER Calls target = FUNCTION(does_free).PARAM(1) methods = free
  -- ParameterAnnotation

  TOOL_CHECKER Calls target = FUNCTION(creates_obligation).PARAM(2).FIELD(x)
  methods = free
  -- ParameterAnnotation with field

  TOOL_CHECKER MustCall target = FUNCTION(creates_obligation).RETURN methods =
  free
  -- ReturnAnnotation

  TOOL_CHECKER Calls target = FUNCTION(does_something).RETURN.FIELD(x) methods =
  free
  -- ReturnAnnotation with field

  TOOL_CHECKER MustCall target = STRUCT(my_struct).FIELD(x) methods = free
  -- StructAnnotation; always has FIELD specifier
  */

  if (chunks[0] != "TOOL_CHECKER") {
    return UndefinedAnnotation;
  }

  if (chunks[1] == "Calls") {
    annoType = AnnotationType::CallsAnnotation;
  } else if (chunks[1] == "MustCall") {
    annoType = AnnotationType::MustCallAnnotation;
  }

  if (annoType == AnnotationType::UndefinedAnnotation) {
    return UndefinedAnnotation;
  }

  std::string methodsString;

  for (int i = 7; i < chunks.size(); i++) {
    methodsString += chunks[i];
  }
  methodsString = dataflow::removeWhitespace(methodsString);

  std::vector<std::string> methodsVector =
      dataflow::splitString(methodsString, ',');
  std::set<std::string> methodsSet(methodsVector.begin(), methodsVector.end());

  std::vector<std::string> targetChunks =
      dataflow::splitString(chunks[4], '.'); // target = ... section
  int targetChunksSize = targetChunks.size();

  std::string targetType = targetChunks[0].substr(
      0, targetChunks[0].find('(')); // equals FUNCTION(...) or STRUCT(...)
  std::string name =
      dataflow::sliceString(targetChunks[0], targetChunks[0].find('(') + 1,
                            targetChunks[0].find(')') - 1);

  if (targetChunksSize == 1 && targetType == "FUNCTION") {
    return new FunctionAnnotation(annoType, methodsSet, name);
  }

  // all structs targets specify a field
  if (targetChunksSize == 2 && targetType == "STRUCT") {
    std::string field =
        dataflow::sliceString(targetChunks[1], targetChunks[1].find('(') + 1,
                              targetChunks[1].find(')') - 1);
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
        returnField = dataflow::sliceString(targetChunks[2],
                                            targetChunks[2].find('(') + 1,
                                            targetChunks[2].find(')') - 1);
      }

      return new ReturnAnnotation(annoType, methodsSet, name, returnField);
    }

    // targetField is PARAM(int)
    int nthParameter = std::stoi(dataflow::sliceString(
        targetField, targetField.find('(') + 1, targetField.find(')') - 1));
    std::string parameterField;

    // there is a parameter field
    if (targetChunksSize == 3) {
      parameterField =
          dataflow::sliceString(targetChunks[2], targetChunks[2].find('(') + 1,
                                targetChunks[2].find(')') - 1);
    }

    return new ParameterAnnotation(annoType, methodsSet, name, parameterField,
                                   nthParameter);
  }

  return UndefinedAnnotation;
}