from DeclParser.DeclType import DeclType


class AnnotateAttr(DeclType):
    def __init__(self, annotation_type: str, annotation_target: type, annotation_methods: str):
        self.__annotation_type = annotation_type
        self.__annotation_target = annotation_target
        self.__annotation_methods = annotation_methods
        super().__init__("AnnotateAttr")

    def get_annotation_type(self):
        return self.__annotation_type

    def get_annotation_target(self):
        return self.__annotation_target

    def get_annotation_methods(self):
        return self.__annotation_methods
