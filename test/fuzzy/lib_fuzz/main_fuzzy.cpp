#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <cassert>

#include "math_custom_fun.h"
#include <fuzzer/FuzzedDataProvider.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    FuzzedDataProvider fdp(data, size);
    auto value = fdp.ConsumeFloatingPointInRange<float>(-1, 1);
    std::cout << "Fuzzed float: " << NextSilicon::nextSiliconSineFP32(value, NextSilicon::FunctionVersion::TAYLOR_C_ORIGINAL) << std::endl;
    std::cout << "Fuzzed float: " << NextSilicon::nextSiliconSineFP32(value, NextSilicon::FunctionVersion::TAYLOR_CPP_OPTIMIZED) << std::endl;
    // assert(1 < 0);

    return 0;
}
