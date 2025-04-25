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
    auto value = fdp.ConsumeFloatingPointInRange<float>(-1, 1);
    NextSilicon::SineArguments sineArgs;
    sineArgs.taylorDegreeEnd = 20;
    auto sineTayOrig =  NextSilicon::nextSiliconSineFP32(value, NextSilicon::FunctionVersion::TAYLOR_C_ORIGINAL);
    auto sineTayOptimized = NextSilicon::nextSiliconSineFP32(value, NextSilicon::FunctionVersion::TAYLOR_CPP_OPTIMIZED, sineArgs);
    auto sineChebPoly = NextSilicon::nextSiliconSineFP32(value, NextSilicon::FunctionVersion::CHEB_POLY, sineArgs);
    auto sineGold = ::sinf(value);

    auto relErrorTayOrig = computeRelativeError(sineGold, sineTayOrig);
    auto relErrorSinetayOpt = computeRelativeError(sineGold, sineTayOptimized);
    auto relErrorSineChebPoly = computeRelativeError(sineGold, sineChebPoly);
    assert(relErrorTayOrig < 1e-5);
    assert(relErrorSinetayOpt < 1e-5);
    assert(relErrorSineChebPoly < 1e-5);

    return 0;
}
