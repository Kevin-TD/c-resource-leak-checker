class Annotation:
    """Represents a user-generated annotation on a struct, method parameter, 
    or method return
    """

    def __init__(self, anno_type: str, target: str, methods: str):
        """

        Args:
            anno_type (str): CalledMethods or MustCall
            target (str): name of function/struct 
            methods (str): list of methods (as a str) specified in the annotation
        """
        self.__anno_type = anno_type
        self.__target = target
        self.__methods = methods

    def get_target(self) -> str:
        return self.__target

    def get_methods(self) -> str:
        return self.__methods
    
    def get_anno_type(self) -> str:
        return self.__anno_type
    
    def target_and_anno_type_equal(self, other: "Annotation") -> bool:
        return self.__anno_type == other.get_anno_type() and self.__target == other.get_target()

    def methods_equal(self, other: "Annotation") -> bool:
        return self.__methods == other.get_methods()

    def to_str(self) -> str:
        return f"{self.__anno_type} target = {self.__target} methods = {self.__methods}"