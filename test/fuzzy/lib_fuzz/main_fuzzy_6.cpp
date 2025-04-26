#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <cassert>

#include "math_custom_fun.h"
#include <fuzzer/FuzzedDataProvider.h>

float computeRelativeError(float goldVal, float compVal)
{
    if (std::abs(goldVal) == 0.0)
    {
        return std::abs(compVal - goldVal);
    }
    return std::abs(compVal - goldVal) / std::abs(goldVal);
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    FuzzedDataProvider fdp(data, size);
    auto value = fdp.ConsumeFloatingPointInRange<float>(3, 4);
    NextSilicon::SineArguments sineArgs;
    sineArgs.degreeEnd = 15;
    auto sineTayOrig =  NextSilicon::nextSiliconSineFP32(value, NextSilicon::FunctionVersion::TAYLOR_C_ORIGINAL);
    auto sineTayOptimized = NextSilicon::nextSiliconSineFP32(value, NextSilicon::FunctionVersion::TAYLOR_CPP_OPTIMIZED, sineArgs);
    auto sineChebPoly = NextSilicon::nextSiliconSineFP32(value, NextSilicon::FunctionVersion::CHEB_POLY, sineArgs);
    auto sineGold = ::sinf(value);

    auto relErrorTayOrig = computeRelativeError(sineGold, sineTayOrig);
    auto relErrorSinetayOpt = computeRelativeError(sineGold, sineTayOptimized);
    auto relErrorSineChebPoly = computeRelativeError(sineGold, sineChebPoly);
    // std::cout << relErrorSineChebPoly << std::endl;
    assert(relErrorTayOrig < 1e2);
    assert(relErrorSinetayOpt < 1e2);
    assert(relErrorSineChebPoly < 1e-3);

    return 0;
}
