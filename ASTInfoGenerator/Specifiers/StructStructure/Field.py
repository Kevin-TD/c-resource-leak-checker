class Field:
    """Represents field of a struct
    """

    def __init__(self, field_name: str, field_index: int):
        self.__field_name = field_name
        self.__field_index = field_index

    def get_field_name(self):
        return self.__field_name

    def get_field_index(self):
        return self.__field_index
