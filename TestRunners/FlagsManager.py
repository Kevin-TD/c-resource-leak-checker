from .Flag import Flag
import sys

sys.path.insert(0, '..')
from TestRunners.Utils import *


class _FlagPairError:
    """private class that is used to store error pairs. users can define a 
    flag pair error such that an exception is thrown when both flags are 
    called. this is used to prevent pair calls such as 
    --no-build-ir and --only-build-ir-for. 
    """

    def __init__(self, flag1_name: str, flag2_name: str, error_message: str):
        self.__flag1_name = flag1_name
        self.__flag2_name = flag2_name
        self.__error_message = error_message

    def get_flag1_name(self) -> str:
        return self.__flag1_name

    def get_flag2_name(self) -> str:
        return self.__flag2_name

    def get_error_message(self) -> str:
        return self.__error_message


class FlagsManager:
    """holds a set of flags the user can call to use as a command line tool
    """

    def __init__(self, usage: str):
        """

        Args:
            usage (str): description describing how to use the bundle of flags
        """
        self.__flags: list[Flag] = []
        self.__usage = usage
        self.__flag_pair_errors: list[_FlagPairError] = []

    def add_flag(self, name: str, alias: str, describe: str, action=lambda x: x):
        """
        Args:
            name (str): name of the flag which typically contains alphabet characters and '-'. 
            the user calls the flag by typing --[name]

            alias (str): alias name for the name which is a single character. 
            the user calls the flag by typing -[alias]

            describe (str): description of what the flag does. should typically be
            available to the user when they call for help or insert a flag incorrectly

            action: lambda function that defines some usage
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

    def add_error_pair(self, flag1_name: str, flag2_name: str, error_message: str):
        """defines an error pair such that if flags with name flag1_name 
        and flag2_name are both called, an exception is thrown with 
        message error_message

        Args:
            flag1_name (str): name of first flag (must have prefix '-' if name or '--' if alias)
            flag2_name (str): name of second flag (must have prefix '-' if name or '--' if alias)
            error_message (str): message listed when both flags are called.
            it's recommend you explain why using that pair causes
            an error or unintuitive output 

        Raises:
            ValueError: at least one flag not found by flag name
        """

        if not self.flag_exists(flag1_name):
            raise ValueError(f"flag '{flag1_name}' not found")

        if not self.flag_exists(flag2_name):
            raise ValueError(f"flag '{flag2_name}' not found")

        self.__flag_pair_errors.append(_FlagPairError(
            flag1_name, flag2_name, error_message))

    def check_for_error_pair(self, args: "list[str]"):
        """checks if an error pair of flags is called in command
        line arguments and raises an error if it does

        Args:
            args (list[str]): command line args potentially consisting of
            flag calls

        Raises:
            ValueError: error raised because an error pair was detected
        """

        for error_pair in self.__flag_pair_errors:
            if error_pair.get_flag1_name() in args and error_pair.get_flag2_name() in args:
                raise ValueError(error_pair.get_error_message())

    __FLAG_NAMES_LEFT_PADDING = 5
    __MAX_DESCRIPTION_LINE_LENGTH = 55

    def to_string(self) -> str:
        """Returns a formatted string to describe the usage and flags
        """

        format_str = f"\nUsage: {self.__usage}\n\nOptions:\n"

        max_flag_length = max(len(flag.get_display_name())
                              for flag in self.__flags) + self.__FLAG_NAMES_LEFT_PADDING

        max_left_padding_until_desc = 1 + max_flag_length + self.__FLAG_NAMES_LEFT_PADDING

        for flag in self.__flags:
            flag_names = ' ' * self.__FLAG_NAMES_LEFT_PADDING + \
                (flag.get_display_name()).ljust(max_flag_length)

            desc = flag.get_describe()

            if (len(desc) > self.__MAX_DESCRIPTION_LINE_LENGTH):
                segmented_desc = split_string_by_n_characters(
                    desc, self.__MAX_DESCRIPTION_LINE_LENGTH)

                new_desc = ""

                for i in range(len(segmented_desc)):
                    new_desc += segmented_desc[i]
                    if i != len(segmented_desc) - 1:
                        new_desc += "\n"
                    new_desc += ' ' * max_left_padding_until_desc

                desc = new_desc

            format_str += f"{flag_names} {desc}\n"

        return format_str
