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

# TODO: add command line arg support to specify if we dont want to 
# call make or  re-make IR. also modify if we want to exclude certain files. 
# TODO: there should be final display of all the test results at the end
# TODO: update annotation tester to be as robust as run_test 

class Command: 
    def __init__(self, name: str, alias: str, describe: str, action = lambda x : x):
        self.__name = "-" + name
        self.__alias = "--" + alias
        self.__describe = describe
        self.__action = action
    
    def get_name(self) -> str:
        return self.__name

    def get_alias(self) -> str:
        return self.__alias

    def get_describe(self) -> str:
        return self.__describe
    
    def get_action(self):
        return self.__action

class CommandManager:
    # usage is description describing how to use bundle of commands
    def __init__(self, usage: str):
        self.commands: list[Command] = []
        self.usage = usage
    
    def add_command(self, name: str, alias: str, describe: str, action: lambda x : x):
        self.commands.append(Command(name, alias, describe, action))

    def command_exists(self, command_name: str) -> bool:
        for command in self.commands:
            if command.get_name() == command_name or command.get_alias() == command_name:
                return True
            
        return False 
    
    def get_command(self, command_name: str) -> Command:
        for command in self.commands:
            if command.get_name() == command_name or command.get_alias() == command_name:
                return command
            
        raise ValueError(f"Command '{command_name}' not found")
    
    def get_commands_str(self) -> str:
        # formatting
        pass

command_manager = CommandManager("python3 ../run_test.py [required test folder name] [optional flags]")
command_manager.add_command(
    "h", "help",
    "Show help"
)
command_manager.add_command(
    "m", "no-make",
    "Does not call 'make' command", 
    lambda file_runner_manager : file_runner_manager.toggle_make_call()
)
command_manager.add_command(
    "b", "no-build-ir", 
    "Does not build IR for any file", 
    lambda file_runner_manager : file_runner_manager.toggle_make_call()
)


options = {
    "usage": "str",
    "commands": {
        "h": {
            "alias": "help",
            "describe": "blah blah blah"
        }, 
        "m": {
            "alias": "no-make",
            "describe": "___",
            "action": lambda file_runner_manager : file_runner_manager.toggle_make_call()
        }, 
        "b": {
            "alias": "no-build-ir",
            "describe": "__",
            "action": lambda file_runner_manager : [file.toggle_ir_generation() for file in file_runner_manager]
        }, 
        "f": {
            "alias": "no-build-ir-for",
            "describe": "__",
            "action": lambda file_runner_manager, file_name: file_runner_manager.get_file(file_name).toggle_ir_generation()
        }, 
        "u" : {
            "alias": "only-build-ir-for", 
            "describe": "__",
            "action": lambda file_runner_manager, file_name : [file.toggle_ir_generation() for file in file_runner_manager.get_all_files_excluding(file_name)]
        }, 

        "r": {
            "alias": "no-run-for", 
            "describe": "___",
            "action": lambda file_runner_manager, file_name: file_runner_manager.get_file(file_name).toggle_test_running()
        }, 
        "n": {
            "alias": "only-run-for",
            "describe": "___",
            "action": lambda file_runner_manager, file_name : [file.toggle_test_running() for file in file_runner_manager.get_all_files_excluding(file_name)]
        }
    } 
}



'''
(plans for command line tool impl)
(should also say for results at the end if we did not build ir or explicitly excluded it or did not make)

syntax: 
python3 ../run_test.py [file name] [flags]
python3 ../run_test.py or python3 ../run_test.py --help or python3 ../run_test.py -h     gives you the help 


typical usage
python3 ../run_test.py simple_ptr_test

extra usage 
python3 ../run_test.py simple_ptr_test --no-make --no-build-ir
python3 ../run_test.py simple_layer_test --only-run-for test1.c
python3 ../run_test.py simple_layer_test --only-run-for layer/test1_again.c

commands
--no-make: does not run make 

--no-build-ir: does not build ir for any files. assumes it exists already and will error if they do not 

--no-build-ir-for [c file name] does not build ir for specified file 
    for like simple_layer_test, if you wanted to ignore test1.c, just enter test1.c. if nested,
    type layer/test1_again.c
--only-build-ir-for [c file name]    can be ran for multiple files. ignores all except the ones specified by --only-build-ir-for

--no-run-for [c file name]
--only-run-for [c file name]

should error for the following combinations:  
• --no-build-ir and --only-build-ir-for [file]
• --no-build-ir-for [A] and --only-build-ir-for [B] where A = B 
• --no-run-for [A] and --only-run-for [B] where A = B


class TestFile:
    string file_name
    bool ir_generation
    bool test_ran 



'''


def find_nth(full_string: str, sub_str: str, n: int) -> int:
    start = full_string.find(sub_str)
    while start >= 0 and n > 1:
        start = full_string.find(sub_str, start+len(sub_str))
        n -= 1
    return start

class TestFile:
    # Assumption: all file paths start with ../test/[test name]/
    def __init__(self, file_path: str):
        self.__file_path = file_path
        self.__file_name = file_path[find_nth(file_path, "/", 3) + 1:]
        self.__ir_will_generate = True
        self.__test_will_run = True

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


# nothing specified; print help
# TODO: make help message and put it here
if len(sys.argv) == 1:
    print("<USAGE MESSAGE>")
    sys.exit(1)

# for sys.argv, 0th = ../run_test.py, 1st = folder name, 2 and beyond = user args

test_folder_name = sys.argv[1]

if (test_folder_name == "-h" or test_folder_name == "--help"):
    print("<USAGE MESSAGE>")
    sys.exit(0)

c_files = get_all_c_files(f"../test/{test_folder_name}")

file_runner_manager = FileRunnerManager()
for c_file in c_files:
    file_runner_manager.add_file(c_file)

sys_arg_iterator = 2
while sys_arg_iterator < len(sys.argv):
    arg = sys.argv[sys_arg_iterator]

    if (arg == "--no-make"):
        # file_runner_manager.toggle_make_call()
        options["commands"]["m"]["action"](file_runner_manager)
        


    elif (arg == "--no-build-ir"):
        for f in file_runner_manager.get_all_files():
            f.toggle_ir_generation()
    elif (arg == "--no-build-ir-for"):
        next_arg = sys.argv[sys_arg_iterator + 1]
        f = file_runner_manager.get_file(next_arg)
        f.toggle_ir_generation()
    elif (arg == "--only-build-ir-for"):
        for f in file_runner_manager.get_all_files_excluding(arg):
            f.toggle_ir_generation()
    elif (arg == "--no-run-for"):
        next_arg = sys.argv[sys_arg_iterator + 1]

        options["commands"]["r"]["action"](file_runner_manager, next_arg)

        # f = file_runner_manager.get_file(next_arg)
        # f.toggle_test_running()
    elif (arg == "--only-run-for"):
        for f in file_runner_manager.get_all_files_excluding(arg):
            f.toggle_test_running()

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
        results.append(f"test {c_file} was not ran")


        

print("---------RUN RESULTS---------")
for result in results:
    print(result)

if commands_did_not_fail:
    sys.exit(0)
else:
    sys.exit(1)
