import sys
import os

# Alias for ../TestRunners/AnnotationTestRunner/run_test.py
# Used for only for convenience (easier to remember and type out
# python ../run_at.py testname than
# python ../TestRunners/AnnotationTestRunner/run_test.py testname)

# checks if cwd is build
if os.path.split(os.getcwd())[1] != "build":
    print(f"ERROR: not in build dir; cwd is {os.getcwd()}")
    sys.exit(1)

args = " ".join(sys.argv[1:])
exit_status = os.system(
    f"python3 ../TestRunners/AnnotationTestRunner/run_test.py {args}")

if exit_status != 0:
    sys.exit(1)
