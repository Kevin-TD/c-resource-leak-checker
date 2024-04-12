import sys
sys.path.insert(0, '..')

from ASTAnalyses.ASTInfo.AnnoStructure.Annotation import Annotation


class AnnotationManager:
    """Wrapper for list of annotations; checks annotations passed in for consistency
    """

    def __init__(self):
        self.__annotations: list[Annotation] = []

    def get_annotations(self) -> "list[Annotation]":
        """returns copy of annotation list
        """
        return self.__annotations.copy()

    def add_annotation(self, anno: Annotation):
        """adds annotation and checks previously added annotations for consistency 

        Raises:
            ValueError: raised if two annotations with the same type and target have differing methods
        """
        for annotation in self.__annotations:
            if annotation.target_and_anno_type_equal(anno):
                if not annotation.methods_equal(anno):
                    raise ValueError(
                        f"Two annotations with same type ('{annotation.anno_type}') and target ('{annotation.target}') with differing methods ('{annotation.methods}' and '{anno.methods}')"
                    )

        if anno not in self.__annotations:
            self.__annotations.append(anno)
