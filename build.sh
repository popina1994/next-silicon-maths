NEXT_SILICON_ROOT_PATH="$(dirname "$(realpath "$0")")"
echo $(realpath "$0")
NEXT_SILICON_RESULTS_PATH=$NEXT_SILICON_ROOT_PATH/results
RUN_PERF_AND_ACCUR=false
RUN_FUZZY_TEST=false
RUN_UNIT_TESTS=false
CLEAN_EVERYTHING=false

mkdir -p build
cd build

for arg in "$@"; do
    case $arg in
        --enable-unit-tests)
            RUN_UNIT_TESTS=true
            shift # remove this arg
            ;;
        --enable-fuzzy-tests)
            RUN_FUZZY_TEST=true
            shift
            ;;
        --enable-pef-accur-tests)
            RUN_PERF_AND_ACCUR=true
            shift
            ;;
        --clean)
            CLEAN_EVERYTHING=true
            shift
            ;;
        *)
            echo "Unknown option: $arg"
            ;;
    esac
done

if [ "$CLEAN_EVERYTHING" = true ]; then
    rm * -rf
fi

#C_COMPILER=/usr/bin/gcc
#CXX_COMPILER=/usr/bin/g++
#FUZZY_C_COMPILER=/usr/bin/afl-gcc
#FUZZY_CXX_COMPILER=/usr/bin/afl-g++

# Compiler paramters
echo $NEXT_SILICON_RESULTS_PATH$
C_COMPILER=/usr/bin/clang
CXX_COMPILER=/usr/bin/clang++
AFL_C_COMPILER=/usr/bin/afl-clang
AFL_CXX_COMPILER=/usr/bin/afl-clang++

# Lib fuzz parameters
LIB_FUZZ_NUM_RUNS=1000
LIB_FUZZ_MAX_LEN=10
LIB_FUZZ_SEED=100

# Build
cmake .. -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER\
    -DCMAKE_AFL_C_COMPILER=$AFL_C_COMPILER -DCMAKE_AFL_CXX_COMPILER=$AFL_CXX_COMPILER
make -j

# Run unit tests
if [ "$RUN_UNIT_TESTS" = true ]; then
    ./Next-Silicon-Maths-test
fi
mkdir -p input
mkdir -p corpus


# Run fuzzying
if [ "$RUN_FUZZY_TEST" = true ]; then
    for i in {1..7}; do
        ./Next-Silicon-Maths-fuzz-main_fuzzy_${i}  -runs=${LIB_FUZZ_NUM_RUNS} -max_len=${LIB_FUZZ_SEED} -seed=$LIB_FUZZ_SEED
    done
fi

if [ "$RUN_PERF_AND_ACCUR" = true ]; then
    ./Next-Silicon-Maths --output ${NEXT_SILICON_RESULTS_PATH} --accuracy --performance
    cd ..
    python3 scripts/extract_accuracy.py
    python3 scripts/extract_accuracy_two_pi.py
    python3 scripts/extract_accuracy_zero_zero_zero.py
    python3 scripts/extract_performance.py
fi


# Run afl: still not fully supported!!!
export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1
# Not yet supported:
#afl-fuzz -i input -o output -- ./Next-Silicon-Maths-afl
