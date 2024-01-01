from Flag import Flag
import sys

sys.path.insert(0, '..')
from TestRunners.Utils import *

class FlagsManager:
    """holds a set of flags the user can call to use as a command line tool
    """
    def __init__(self, usage: str):
        """

        Args:
            usage (str): description describing how to use the bundle of flagss
        """
        self.__flags: list[Flag] = []
        self.__usage = usage
    
    def add_flag(self, name: str, alias: str, describe: str, action = lambda x : x):
        """
        Args:
            name (str): name of the flag which typically contains alphabet characters and '-'. 
            the user calls the flag by typing --[name]

            alias (str): alias name for the name which is a single character. 
            the user calls the flag by typing -[alias]

            describe (str): description of what the flag does. should typically be
            available to the user when they call for help or insert a flag incorrectly

            action: lambda function that can have 0, 1, or 2 parameters. if there are 0 parameters, 
            no action is done. if there is 1 parameter, a TestFilesManager reference is passed 
            into that first parameter. if there is a second 
            parameter, a TestFile reference is passed into that second parameter. these can be used 
            to enforce an action, such as toggling IR generation for all files or toggling IR 
            generation for a specific file. 
            nothing is done with its return. 
        """
        self.__flags.append(Flag(name, alias, describe, action))

    def flag_exists(self, flag_name: str) -> bool:
        for flag in self.__flags:
            if flag.get_name() == flag_name or flag.get_alias() == flag_name:
                return True
            
        return False 
    
    def get_flag(self, flag_name: str) -> Flag:
        for flag in self.__flags:
            if flag.get_name() == flag_name or flag.get_alias() == flag_name:
                return flag
            
        raise ValueError(f"flag '{flag_name}' not found")
    
    __FLAG_NAMES_LEFT_PADDING = 5
    __MAX_DESCRIPTION_LINE_LENGTH = 55
    def to_string(self) -> str:
        """Returns a formatted string to describe the usage and flags
        """

        format_str = f"\nUsage: {self.__usage}\n\nOptions:\n"
        
        max_flag_length = max(len(flag.get_display_name()) for flag in self.__flags) + self.__FLAG_NAMES_LEFT_PADDING

        max_left_padding_until_desc = 1 + max_flag_length + self.__FLAG_NAMES_LEFT_PADDING

        for flag in self.__flags:
            flag_names = ' ' * self.__FLAG_NAMES_LEFT_PADDING + (flag.get_display_name()).ljust(max_flag_length)

            desc = flag.get_describe()

            
            if (len(desc) > self.__MAX_DESCRIPTION_LINE_LENGTH):
                segmented_desc = split_string_by_n_characters(desc, self.__MAX_DESCRIPTION_LINE_LENGTH)

                new_desc = ""
                
                for i in range(len(segmented_desc)):
                    new_desc += segmented_desc[i]
                    if i != len(segmented_desc) - 1:
                        new_desc += "\n"
                    new_desc += ' ' * max_left_padding_until_desc
                
                desc = new_desc


            format_str += f"{flag_names} {desc}\n"
                
        return format_str