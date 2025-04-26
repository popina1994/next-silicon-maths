# Installation
## Dependencies
The following subsection contains the list of required libraries and packages need to be installed in order to run the solution.
# Application
1. Bash (at least 5.1.16)
2. C++ implementation
    1. gcc (at least 11.4.0)
    2. cmake (at least 3.22.1)
    3. boost (at least 1.74.0)
    4. Fuzzy library afl :
        1. sudo apt install afl
    5. Fuzzy library libFuzzer

# Plotting
1. python 3 (at least 3.10.12)
2. all python packages listed in pip requirements.txt
## Reproducibility
1. Install all required dependencies
2. Update build.sh: compilers, seeds, uncomment either unit tests, fuzzy tests or the program itself
3. from the source project path bash build.sh  --enable-unit-tests --enable-fuzzy-tests --enable-pef-accur-tests --clean
