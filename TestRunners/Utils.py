import os

TERMINAL_TEXT_STYLES = {
    "BOLD_ON": "\033[1m",
    "BOLD_OFF": "\033[22m",
    "RED_COLOR": "\033[31m",
    "GREEN_COLOR": "\033[32m",
    "RESET_COLOR": "\033[0m",
}


def split_string_by_n_characters(input_string: str, n: int) -> "list[str]":
    """splits a string every n characters into a new array. does not split words apart. 

    Args:
        input_string (str)
        n (int)

    Returns:
        list[str]
    """

    words = input_string.split(' ')

    result = []
    current_chunk = ''
    for word in words:
        if len(current_chunk) + len(word) <= n:
            current_chunk += (' ' if current_chunk else '') + word
        else:
            result.append(current_chunk)
            current_chunk = word

    if current_chunk:
        result.append(current_chunk)

    return result


def find_nth(full_string: str, sub_str: str, n: int) -> int:
    """returns the index of the nth occurrence of sub_str in full_string

    Args:
        full_string (str)
        sub_str (str)
        n (int)

    Returns:
        int
    """
    start = full_string.find(sub_str)
    while start >= 0 and n > 1:
        start = full_string.find(sub_str, start+len(sub_str))
        n -= 1
    return start


def get_all_c_files(folder_path: str, collected_files=[]) -> "list[str]":
    """returns a list of all c files in a folder, including those in subdirectories

    Args:
        folder_path (str): path to a folder directory

    Returns:
        list[str]
    """
    with os.scandir(folder_path) as entries:
        for entry in entries:
            if entry.is_file() and entry.name.endswith(".c"):
                collected_files.append(f"{folder_path}/{entry.name}")

            elif entry.is_dir():
                get_all_c_files(f"{folder_path}/{entry.name}", collected_files)

    return collected_files
