class Flag: 
    """represents a comnand line flag a user can input when calling run_test.py
    """
    def __init__(self, name: str, alias: str, describe: str, action = lambda x : x):
        """
        Args:
            name (str): name of the flag which typically contains alphabet characters and '-'. 
            the user calls the flag by typing --[name]

            alias (str): alias name for the name which is a single character. 
            the user calls the flag by typing -[alias]

            describe (str): description of what the flag does. should typically be
            available to the user when they call for help or insert a flag incorrectly

            action: lambda function that can have 0, 1, or 2 parameters. if there are 0 parameters, 
            no action is done. if there is 1 parameter, a FileRunnerManager instance is passed 
            into that first parameter. if there is a second 
            parameter, a TestFile instance is passed into that second parameter. these can be used 
            to enforce an action, such as toggling IR generation for all files or toggling IR 
            generation for a specific file. 
            nothing is done with its return. 
        """
        self.__name: str = "-" + name
        self.__alias: str  = "--" + alias
        self.__describe: str = describe
        self.__action = action
    
    def get_name(self) -> str:
        return self.__name

    def get_alias(self) -> str:
        return self.__alias

    def get_display_name(self) -> str:
        """Returns the name and its alias as a string for user 
        display purposes. e.g., showing the user the flag option 
        "-m, --no-make". 
        """
        return self.__name + ", " + self.__alias

    def get_describe(self) -> str:
        return self.__describe
    
    def get_action(self):
        return self.__action