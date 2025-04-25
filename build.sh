mkdir -p build
cd build
rm * -rf

#C_COMPILER=/usr/bin/gcc
#CXX_COMPILER=/usr/bin/g++
#FUZZY_C_COMPILER=/usr/bin/afl-gcc
#FUZZY_CXX_COMPILER=/usr/bin/afl-g++

C_COMPILER=/usr/bin/clang
CXX_COMPILER=/usr/bin/clang++
AFL_C_COMPILER=/usr/bin/afl-clang
AFL_CXX_COMPILER=/usr/bin/afl-clang++

cmake .. -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER\
    -DCMAKE_AFL_C_COMPILER=$AFL_C_COMPILER -DCMAKE_AFL_CXX_COMPILER=$AFL_CXX_COMPILER
make

#./Next-Silicon-Maths
#ctest --output-on-failure
mkdir -p input
mkdir -p corpus
echo hello > corpus/seed.txt
echo "HOHO"
./Next-Silicon-Maths-fuzz corpus/

export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1
#afl-fuzz -i input -o output -- ./Next-Silicon-Maths-afl
