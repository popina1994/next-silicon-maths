mkdir -p build
cd build
#rm * -rf

#C_COMPILER=/usr/bin/gcc
#CXX_COMPILER=/usr/bin/g++
#FUZZY_C_COMPILER=/usr/bin/afl-gcc
#FUZZY_CXX_COMPILER=/usr/bin/afl-g++

# Compiler paramters
C_COMPILER=/usr/bin/clang
CXX_COMPILER=/usr/bin/clang++
AFL_C_COMPILER=/usr/bin/afl-clang
AFL_CXX_COMPILER=/usr/bin/afl-clang++

# Lib fuzz parameters
LIB_FUZZ_NUM_RUNS=10
LIB_FUZZ_MAX_LEN=10
LIB_FUZZ_SEED=15

# Build
cmake .. -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER\
    -DCMAKE_AFL_C_COMPILER=$AFL_C_COMPILER -DCMAKE_AFL_CXX_COMPILER=$AFL_CXX_COMPILER
make

# Run regular file
#./Next-Silicon-Maths
# ctest --output-on-failure
# Run unit tests
./Next-Silicon-Maths-test
mkdir -p input
mkdir -p corpus


# Run fuzzying
./Next-Silicon-Maths-fuzz-main_fuzzy  -runs=${LIB_FUZZ_NUM_RUNS} -max_len=${LIB_FUZZ_SEED} -seed=$LIB_FUZZ_SEED

# Run afl: still not fully supported
export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1
#afl-fuzz -i input -o output -- ./Next-Silicon-Maths-afl
