import os
import sys

if os.path.split(os.getcwd())[1] != "build":
    print(f"WARNING: not in build dir; cwd is {os.getcwd()}")
    sys.exit(1)

sys.path.insert(0, '..')
from TestRunners.FlagsManager import FlagsManager
from PassTestFilesManager import PassTestFilesManager
from PassTestRunner import PassTestRunner


if __name__ == "__main__":
    '''
    Usage examples: 
    python3 ../run_pt.py [file name] [flags]
    python3 ../run_pt.py -h (to get help) 

    python3 ../run_pt.py simple_ptr_test
    python3 ../run_pt.py simple_ptr_test --no-make --no-build-ir
    python3 ../run_pt.py simple_layer_test --only-run-for test1.c
    python3 ../run_pt.py simple_layer_test --only-run-for layer/test1_again.c
    '''

    flags_manager = FlagsManager(
        "python3 ../run_pt.py [required test folder name] [optional flags]")
    flags_manager.add_flag(
        "h", "help",
        "Show help"
    )

    flags_manager.add_flag(
        "m", "no-make",
        "Does not call 'make' command",
        lambda pass_test_files_manager: pass_test_files_manager.toggle_make_call()
    )

    flags_manager.add_flag(
        "b", "no-build-ir",
        "Does not build IR for any file",
        lambda pass_test_files_manager: [file.toggle_ir_generation(
        ) for file in pass_test_files_manager.get_all_files()]
    )

    flags_manager.add_flag(
        "f", "no-build-ir-for",
        "Does not build IR for the specified C file. Can be called multiple times. C file must be specified in the succeeding argument.",
        lambda pass_test_files_manager, file_name: pass_test_files_manager.get_file(
            file_name).toggle_ir_generation()

    )
    flags_manager.add_flag(
        "u", "only-build-ir-for",
        "Only builds ir for specified C file. c file must be specified in the succeeding argument. Note that c files will still be run unless otherwise disallowed (use -n)",
        lambda pass_test_files_manager, file_name: [file.toggle_ir_generation(
        ) for file in pass_test_files_manager.get_all_files_excluding(file_name)]
    )

    flags_manager.add_flag(
        "r", "no-run-for",
        "Does not run specified C file. Can be called multiple times. C file must be specified in the succeeding argument",
        lambda pass_test_files_manager, file_name: pass_test_files_manager.get_file(
            file_name).toggle_test_running()
    )

    flags_manager.add_flag(
        "n", "only-run-for",
        "Only runs specified C file. C file must be specified in the succeeding argument. Note that IR will still generate unless otherwise disallowed (use -u)",
        lambda pass_test_files_manager, file_name: [file.toggle_test_running(
        ) for file in pass_test_files_manager.get_all_files_excluding(file_name)]
    )

    flags_manager.add_error_pair("--no-build-ir", "--only-build-ir-for",
                                 "Flag effects can undo each other; e.g., `--no-build-ir --only-build-ir-for file.c` will first toggle ir for `file.c` (from --no-build-ir) then toggle ir for `file.c` again (from --only-build-ir-for)")
    flags_manager.add_error_pair("--no-run-for", "--only-run-for",
                                 "Flag effects can undo each other; e.g., `--no-run-for fileA.c --only-run-for fileB.c` will first toggle running for `fileA.c` (from --no-run-for) then toggle running for `fileA.c` again (from --only-run-for fileB.c). happens because `--only-run-for fileB.c` toggles running for all files except `fileB.c` ")

    pass_test_runner = PassTestRunner(flags_manager, PassTestFilesManager())

    exit_status = pass_test_runner.manage_input_and_run_tests(sys.argv)
    sys.exit(exit_status)
