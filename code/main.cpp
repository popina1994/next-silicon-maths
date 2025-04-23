#include "math_custom_fun.h"

#include <iostream>
#include <cmath>
#include <iomanip>
#include <utility>
#include <numbers>
#include <limits>
std::tuple<float, float, float, float> evalPrecision(float val)
{
    auto sinValCustom = NextSilicon::nextSiliconSineFP32(val, 50);
    auto sinVal = std::sin(val);
    auto absError = std::abs(sinVal - sinValCustom);
    auto relError = absError / sinVal;
    return {absError, relError, sinValCustom, sinVal};
}

int main(void)
{
    std::cout << std::setprecision(std::numeric_limits<float>::digits10 ) << std::scientific;
    for (auto val = -std::numbers::pi_v<float>; val < std::numbers::pi_v<float>; val += 0.1f)
    {
        auto [absError, relError, sinVal, sinValCustom] = evalPrecision(val);
        std::cout << "VAL: " << val << " " << "SINVAL: " << sinVal << " " << "SINVAL CUSTOM: " << sinValCustom << " " << absError << " " << relError << std::endl;
    }

    return 0;
}