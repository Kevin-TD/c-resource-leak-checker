# c-resource-leak-checker

This analysis runs with docker; here are the steps to setup the analysis: 

1. Clone the repo and set directory to the repo's 
2. Pull the docker image: ```docker pull cis547/cis547-base:latest```
3. Run docker in interactive mode: ```docker run -it -v $(pwd):/$(basename $(pwd)) -w /$(basename $(pwd))  cis547/cis547-base:latest```
- Note: if any files in your cwd have spaces in them, then step 3 will likely fail as the ```$(basename $(pwd))``` command will not work as intended. This is an issue to fix later, but for now, just make sure you don't have any spaces in your folders
4. Run the following to get opt working: ```export LD_LIBRARY_PATH=/opt/mirtk/lib:${LD_LIBRARY_PATH}```
5. Make (```mkdir build```) and set directory to build (```cd build```)
6. Build Clang, LLVM, other important stuff with ```cmake ..```

To run a pass test call ```python3 ../run_pt.py test_name``` for a test in the ./test directory. test_name is based on directory name. 
E.g., ```python3 ../run_pt.py simple_anno_test```
- See command line options by calling ```python3 ../run_pt.py --help```

To run an annotation test call ```python3 ../run_at.py test_name```
- See command line options by calling ```python3 ../run_at.py --help```

To run every pass test call ```python3 ../whole_test_run.py PassTestRunner Passes```
- Add ```--no-make```flag if your analysis is already built (optional, but it speeds up the analysis a bit)

To run every annotation test call ```python3 ../whole_test_run.py AnnotationTestRunner Annotations```