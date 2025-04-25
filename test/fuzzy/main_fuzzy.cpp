#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iomanip>
#include "math_custom_fun.h"
#include <fuzzer/FuzzedDataProvider.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    FuzzedDataProvider fdp(data, size);

    auto value = fdp.ConsumeFloatingPoint<float>();

    // Use the double value
    std::cout << "Fuzzed double: " << fp32_custom_sine(value) << std::endl;

    return 0;
}
