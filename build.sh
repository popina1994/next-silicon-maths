mkdir -p build
cd build
# rm * -rf
C_COMPILER=/usr/bin/gcc
CXX_COMPILER=/usr/bin/g++
FUZZY_C_COMPILER=/usr/bin/afl-gcc
FUZZY_CXX_COMPILER=/usr/bin/afl-g++
cmake .. -DCMAKE_C_COMPILER=$FUZZY_C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER\
    -DCMAKE_FUZZY_C_COMPILER=$FUZZY_C_COMPILER -DCMAKE_FUZZY_CXX_COMPILER=$FUZZY_CXX_COMPILER
make -j

./Next-Silicon-Maths
#ctest --output-on-failure
mkdir -p input
echo "-1" > input/test_1.txt
echo "0" > input/test_2.txt

export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1
#afl-fuzz -i input -o output -- ./Next-Silicon-Maths-afl
