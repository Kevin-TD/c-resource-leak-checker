#! python3 ../run_test.py <folder name in /test>
# must be in build dir
# e.g., python3 ../run_test.py nesting_test

# takes a test folder in the /test directory and runs every .c
# file in it, including those in subdirectories

import os 
import sys 

# checks if cwd is build
if os.path.split(os.getcwd())[1] != "build":
    print(f"WARNING: not in build dir; cwd is {os.getcwd()}")
    sys.exit(1)

# TODO: there should be final display of all the test results at the end
# TODO: update annotation tester to be as robust as run_test 
# TODO: turn run_test (and good number of other python files in codebase) into separate files. run_test.py should do if __name__ == main 

# splits a string every n characters into a new array. does not split words apart. 
def split_string_by_n_characters(input_string: str, n: int):
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

class Flag: 
    def __init__(self, name: str, alias: str, describe: str, action = lambda x : x):
        self.__name = "-" + name
        self.__alias = "--" + alias
        self.__describe = describe
        self.__action = action
    
    def get_name(self) -> str:
        return self.__name

    def get_alias(self) -> str:
        return self.__alias

    def get_display_name(self) -> str:
        return self.__name + ", " + self.__alias

    def get_describe(self) -> str:
        return self.__describe
    
    def get_action(self):
        return self.__action

class FlagManager:
    # usage is description describing how to use bundle of flags
    def __init__(self, usage: str):
        self.__flags: list[Flag] = []
        self.__usage = usage
    
    def add_flag(self, name: str, alias: str, describe: str, action = lambda x : x):
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
    
    def to_string(self) -> str:
        format_str = f"\nUsage: {self.__usage}\n\nOptions:\n"

        flag_names_left_padding = 5 # TODO: class const with double underscore 
        max_description_length = 55 # TODO: class const with double underscore
        
        max_flag_length = max(len(flag.get_display_name()) for flag in self.__flags) + flag_names_left_padding

        max_left_padding_until_desc = 1 + max_flag_length + flag_names_left_padding

        for flag in self.__flags:
            flag_names = ' ' * flag_names_left_padding + (flag.get_display_name()).ljust(max_flag_length)

            desc = flag.get_describe()

            
            if (len(desc) > max_description_length):
                segmented_desc = split_string_by_n_characters(desc, max_description_length)

                new_desc = ""
                
                for i in range(len(segmented_desc)):
                    new_desc += segmented_desc[i]
                    if i != len(segmented_desc) - 1:
                        new_desc += "\n"
                    new_desc += ' ' * max_left_padding_until_desc
                
                desc = new_desc


            format_str += f"{flag_names} {desc}\n"
                
        return format_str
    


flag_manager = FlagManager("python3 ../run_test.py [required test folder name] [optional flags]")
flag_manager.add_flag(
    "h", "help",
    "Show help"
)

flag_manager.add_flag(
    "m", "no-make",
    "Does not call 'make' command", 
    lambda file_runner_manager : file_runner_manager.toggle_make_call()
)

flag_manager.add_flag(
    "b", "no-build-ir", 
    "Does not build IR for any file. Combined with --only-build-ir-for may result in undesirable output as these commands are opposites. ", 
    lambda file_runner_manager : [file.toggle_ir_generation() for file in file_runner_manager.get_all_files()]
)

flag_manager.add_flag(
    "f", "no-build-ir-for", 
    "Does not build IR for the specified c file. Can be called multiple times. c file must be specified in the succeeding argument, or else there will be an error.", 
    lambda file_runner_manager, file_name: file_runner_manager.get_file(file_name).toggle_ir_generation()

)
flag_manager.add_flag(
    "u", "only-build-ir-for", 
    "Only builds ir for specified c file. c file must be specified in the succeeding argument, or else there will be an error. Note that c files will still be ran unless otherwise disallowed (use -n). Combined with --no-build-ir may result in undesirable output as these commands are opposites.",
    lambda file_runner_manager, file_name : [file.toggle_ir_generation() for file in file_runner_manager.get_all_files_excluding(file_name)]
)

flag_manager.add_flag(
    "r", "no-run-for", 
    "Does not specified c file. Can be called multiple times. c file must be specified in the succeeding argument, or else there will be an error.", 
    lambda file_runner_manager, file_name: file_runner_manager.get_file(file_name).toggle_test_running()   
)

flag_manager.add_flag(
    "n", "only-run-for", 
    "Only runs specified c file. c file must be specified in the succeeding argument, or else there will be an error. Note that IR will still generate unless otherwise disallowed (use -u).",
    lambda file_runner_manager, file_name : [file.toggle_test_running() for file in file_runner_manager.get_all_files_excluding(file_name)]
)


'''
syntax: 
python3 ../run_test.py [file name] [flags]
python3 ../run_test.py or python3 ../run_test.py --help or python3 ../run_test.py -h to get help

example usage
python3 ../run_test.py simple_ptr_test
python3 ../run_test.py simple_ptr_test --no-make --no-build-ir
python3 ../run_test.py simple_layer_test --only-run-for test1.c
python3 ../run_test.py simple_layer_test --only-run-for layer/test1_again.c
'''


def find_nth(full_string: str, sub_str: str, n: int) -> int:
    start = full_string.find(sub_str)
    while start >= 0 and n > 1:
        start = full_string.find(sub_str, start+len(sub_str))
        n -= 1
    return start



class TestFile:
    INIT_TEST_CONDITIONS = {
        "IR_WILL_GENERATE": True, 
        "TEST_WILL_RUN": True, 
    }
    
    # Assumption: all file paths start with ../test/[test name]/
    def __init__(self, file_path: str):
        self.__file_path = file_path
        self.__file_name = file_path[find_nth(file_path, "/", 3) + 1:]
        self.__ir_will_generate = self.INIT_TEST_CONDITIONS["IR_WILL_GENERATE"]
        self.__test_will_run = self.INIT_TEST_CONDITIONS["TEST_WILL_RUN"]

    def toggle_ir_generation(self):
        self.__ir_will_generate = not self.__ir_will_generate
    
    def toggle_test_running(self):
        self.__test_will_run = not self.__test_will_run

    def ir_will_generate(self) -> bool:
        return self.__ir_will_generate

    def test_will_run(self) -> bool:
        return self.__test_will_run

    def get_file_path(self) -> str:
        return self.__file_path

    # file name is file_path but with ../test/[test name]/ removed. used 
    # when the user references a file name to to include/exclude
    def get_file_name(self) -> str:
        return self.__file_name

class FileRunnerManager:
    def __init__(self):
        self.__test_files: list[TestFile] = []
        self.__make_will_be_called = True
    
    def add_file(self, file_path: str):
        self.__test_files.append(TestFile(file_path))
    
    # file name is file_path but with ../test/[test name]/ removed 
    def get_file(self, file_name: str) -> TestFile:
        for test_file in self.__test_files:
            if test_file.get_file_name() == file_name:
                return test_file
        
        raise FileNotFoundError(f"File '{file_name}' not found")
    
    def get_all_files_excluding(self, file_name: str) -> "list[TestFile]":
        test_files = []
        file_found = False
        for test_file in self.__test_files:
            if test_file.get_file_name() != file_name:
                test_files.append(test_file)
            else: 
                file_found = True
        
        if not file_found:
            raise FileNotFoundError(f"File '{file_name}' not found")
        
        return test_files

    # returns of a list of references to the files. this list of 
    # references cannot modify this original test file list.  
    def get_all_files(self) -> "list[TestFile]":
        test_files = []
        for test_file in self.__test_files:
            test_files.append(test_file)
        
        return test_files
    
    def make_will_be_called(self) -> bool:
        return self.__make_will_be_called
    
    def toggle_make_call(self):
        self.__make_will_be_called = not self.__make_will_be_called

    # file name is file_path but with ../test/[test name]/ removed 
    def toggle_ir_for(self, file_name: str):
        for test_file in self.__test_files:
            if test_file.get_file_name() == file_name:
                test_file.toggle_ir_generation()
                return
        
        raise FileNotFoundError(f"File '{file_name}' not found")
        

    # file name is file_path but with ../test/[test name]/ removed
    def toggle_run_for(self, file_name: str):
        for test_file in self.__test_files:
            if test_file.get_file_name() == file_name:
                test_file.toggle_test_running()
                return
        
        raise FileNotFoundError(f"File '{file_name}' not found")


def get_all_c_files(folder_path: str, collected_files = []):
    with os.scandir(folder_path) as entries:
        for entry in entries:
            if entry.is_file() and entry.name.endswith(".c"):
                collected_files.append(f"{folder_path}/{entry.name}")

            elif entry.is_dir():
                get_all_c_files(f"{folder_path}/{entry.name}", collected_files)

    return collected_files


if len(sys.argv) == 1:
    print(flag_manager.to_string())
    sys.exit(1)

# for sys.argv, 0th = ../run_test.py, 1st = folder name, 2 and beyond = user args

test_folder_name = sys.argv[1]

if (test_folder_name == "-h" or test_folder_name == "--help"):
    print(flag_manager.to_string())
    sys.exit(0)

c_files = get_all_c_files(f"../test/{test_folder_name}")

file_runner_manager = FileRunnerManager()
for c_file in c_files:
    file_runner_manager.add_file(c_file)

sys_arg_iterator = 2
while sys_arg_iterator < len(sys.argv):
    arg = sys.argv[sys_arg_iterator]

    if (flag_manager.flag_exists(arg)):
        flag = flag_manager.get_flag(arg)

        action_arg_count = flag.get_action().__code__.co_argcount

        if (action_arg_count == 1):
            flag.get_action()(file_runner_manager)
        elif (action_arg_count == 2):
            next_arg = sys.argv[sys_arg_iterator + 1]
            flag.get_action()(file_runner_manager, next_arg)
            sys_arg_iterator += 2
            continue

    else:
        raise ValueError(f"Flag {arg} not found. {flag_manager.to_string()}")

    sys_arg_iterator += 1

print(f"Calling Make: {file_runner_manager.make_will_be_called()}")

results = []

if (file_runner_manager.make_will_be_called()):
    make_status = os.system("make")

    if make_status != 0:
        sys.exit(make_status)
else:
    results.append("make not called")



commands_did_not_fail = True


for c_file in c_files:

    c_file_as_ll = c_file.replace(".c", ".ll")

    f = file_runner_manager.get_file(c_file[find_nth(c_file, "/", 3) + 1:])

    make_ir_command = f"clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c {c_file} -o {c_file_as_ll}"
    run_test_command = f"opt -load CodeAnalyzer.so -CodeAnalyzer {c_file_as_ll}"

    print(f"Making IR: {f.ir_will_generate()}")

    if f.ir_will_generate():
        ir_generate_exit_status = os.system(make_ir_command)

        if ir_generate_exit_status != 0:
            commands_did_not_fail = False
            results.append(f"test {c_file} failed @ IR generation - exit status {ir_generate_exit_status}")
            continue
    
    print(f"Running Test: {f.test_will_run()}")

    if f.test_will_run():
        test_run_exit_status = os.system(run_test_command)

        if test_run_exit_status != 0:
            commands_did_not_fail = False
            results.append(f"test {c_file} failed @ running - exit status {test_run_exit_status}")
        else:
            result = f"test {c_file} did not fail"
            if not f.ir_will_generate():
                result += " | ir not generated"

            results.append(result)
    else:
        result = f"test {c_file} was not ran"
        if not f.ir_will_generate():
            result += " | ir not generated"

        results.append(result)



        

print("---------RUN RESULTS---------")
for result in results:
    print(result)

if commands_did_not_fail:
    sys.exit(0)
else:
    sys.exit(1)
