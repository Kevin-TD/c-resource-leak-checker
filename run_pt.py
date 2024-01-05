import sys
import os

# Alias for ../TestRunners/PassTestRunners/run_test.py

args = " ".join(sys.argv[1:])
exit_status = os.system(
    f"python3 ../TestRunners/PassTestRunner/run_test.py {args}")

if exit_status != 0:
    sys.exit(1)
