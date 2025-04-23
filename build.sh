source /opt/intel/oneapi/2025.1/oneapi-vars.sh
mkdir -p build
cd build
rm * -rf
cmake ..
make -j

./Next-Silicon-Maths
# ctest