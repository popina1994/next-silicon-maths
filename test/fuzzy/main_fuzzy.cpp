// #include <cmath>
// #include <cstdint>
// #include <cstring>
// #include <iostream>
// #include <iomanip>
// #include "math_custom_fun.h"

// uint8_t charToNum(uint8_t num)
// {
//     if (num >= '0' and num <= '9')
//     {
//         return num - '0';
//     }
//     else if (num >= 'A' and num <= 'F')
//     {
//         return num - 'A' + 10;
//     }
//     return num - 'a' + 10;
// }

// void memCpyMy(uint8_t* dst, const uint8_t* src, int size)
// {
//     // std::cout << size << std::endl;
//     for (int idx = 0; idx < size; idx++)
//     {
//         uint8_t pos1 = charToNum(src[2 * idx]);
//         uint8_t pos2 = charToNum(src[2 * idx + 1]);
//         // std::cout << int(dst[size - idx - 1]) << std::endl;
//         dst[size - idx - 1] = (pos1  << 4) | pos2;
//         // std::cout << idx << " " << (int)dst[size - idx - 1] << " " <<  (int)pos1 << " " << (int)pos2 << std::endl;
//     }
// }
// void fuzzTarget(const uint8_t *data, size_t size) {
//     // Ensure there are enough bytes to interpret as a float
//     if (size * 2 < sizeof(float)) {
//         std::cout << "Not enough data to interpret as float." << std::endl;
//         return;
//     }
//     for (int idx = 0; idx < size; idx++)
//     {
//         // std::cout << idx << " " << std::hex << data[idx] << " " << std::endl;
//     }
//     float input_value;
//     memCpyMy(reinterpret_cast<uint8_t*>(&input_value), data, sizeof(input_value));
//     // std::cout << "Interpreted float value: " << input_value <<  std::endl;
//     if (input_value <= 1e-20f and input_value >= -1.0e-20f)
//     {
//         // *t  = 1;
//     }
//     float result = input_value * input_value;
//     std::cout << "Squared value: " << result << std::endl;
// }

// int main(int argc, char **argv) {
//     // AFL will pass input data as stdin

//     if (argc > 1) {
//         fuzzTarget(reinterpret_cast<uint8_t*>(argv[1]), strlen(argv[1]));
//     }

//     return 0;
// }

#include <cstddef>
#include <cstdint>
#include <cstring>

// This is the function LibFuzzer will call repeatedly
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Example logic: crash if input is "crash"
    if (size >= 5 && std::memcmp(data, "crash", 5) == 0) {
        __builtin_trap(); // or abort();
    }
    return 0;
}

