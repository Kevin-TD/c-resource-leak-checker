import sys

sys.path.insert(0, '..')
from TestRunners.FlagsManager import FlagsManager
from AnnotationTestFilesManager import AnnotationTestFilesManager
from AnnotationTestRunner import AnnotationTestRunner


if __name__ == "__main__":
    '''
    Usage Examples: 
    python3 ../run_at.py [file name] [flags]
    python3 ../run_at.py -h to get help

    python3 ../run_at.py includes_test
    python3 ../run_at.py includes_test --no-run-for subdir/runner.c
    python3 ../run_at.py includes_test --only-run-for call_structs.c
    python3 ../run_at.py includes_test --no-run-for subdir/runner.c
    '''

    flags_manager = FlagsManager(
        "python3 ../run_at.py [required test folder name] [optional flags]")
    flags_manager.add_flag(
        "h", "help",
        "Show help"
    )

    flags_manager.add_flag(
        "r", "no-run-for",
        "Does not check annotations for specified c file. Can be called multiple times. c file must be specified in the succeeding argument. Combined with --only-run-for may result in undesirable output.",
        lambda annotation_test_files_manager, file_name: annotation_test_files_manager.get_file(
            file_name).toggle_test_running()
    )

    flags_manager.add_flag(
        "n", "only-run-for",
        "Only runs specified c file. c file must be specified in the succeeding argument. Combined with --no-run-for may result in undesireable output.",
        lambda annotation_test_files_manager, file_name: [file.toggle_test_running(
        ) for file in annotation_test_files_manager.get_all_files_excluding(file_name)]
    )

    annotation_test_runner = AnnotationTestRunner(
        flags_manager, AnnotationTestFilesManager())

    exit_status = annotation_test_runner.manage_input_and_run_tests(sys.argv)
    sys.exit(exit_status)
