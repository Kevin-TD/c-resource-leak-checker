class Parameter:
    """Represents a parameter of a function.
    """
    def __init__(self, index: int, param_type: str):
        self.__index = index
        self.__param_type = param_type
    
    def get_index(self):
        return self.__index
    
    def get_param_type(self):
        return self.__param_type