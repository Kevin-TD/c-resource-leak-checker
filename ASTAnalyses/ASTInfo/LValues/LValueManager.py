from ASTAnalyses.ASTInfo.LValues.LValue import LValue


class LValueManager:
    """Wrapper for a collection of l-value references
    """

    def __init__(self):
        self.l_values: list[LValue] = []

    def add_l_value(self, lhs_name: str, line_number: int):
        new_l_value_ref = LValue(lhs_name, line_number)
        self.l_values.append(new_l_value_ref)

    # gets l_value based off line_number. returns None if not found
    def get_l_value(self, line_number: int):
        for l_value in self.l_values:
            if l_value.get_line_number() == line_number:
                return l_value

        return None
