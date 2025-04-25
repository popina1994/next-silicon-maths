#include "math_custom_fun.h"

#include <iostream>
#include <cmath>
#include <iomanip>
#include <utility>
#include <numbers>
#include <limits>
#include<vector>

std::tuple<float, float, float, float> evalPrecision(float val)
{
    auto sinValCustom = NextSilicon::nextSiliconSineFP32Taylor(val, 5);
    auto sinVal = std::sin(val);
    auto absError = std::abs(sinVal - sinValCustom);
    auto relError = absError / sinVal;
    return {absError, relError, sinValCustom, sinVal};
}

int main(int argc, char* argv[])
{
    std::cout << std::setprecision(std::numeric_limits<float>::digits10 ) << std::scientific;

    std::vector<float> vVals = {2 * std::numbers::pi_v<float>, 3 * -std::numbers::pi_v<float> / 2,
        -std::numbers::pi_v<float>, -std::numbers::pi_v<float>/2, -0.f, 0.f,
        std::numbers::pi_v<float> / 2, std::numbers::pi_v<float>,
        3 * std::numbers::pi_v<float> / 2, 2 * std::numbers::pi_v<float>};


    for (auto val = -std::numbers::pi_v<float>; val < std::numbers::pi_v<float>; val += 0.1f)
    {
        auto [absError, relError, sinVal, sinValCustom] = evalPrecision(val);
        std::cout << "VAL: " << val << " " << "SINVAL: " << sinVal << " " << "SINVAL CUSTOM: " << sinValCustom << " " << absError << " " << relError << std::endl;
    }

    std::cout << std::setprecision(std::numeric_limits<float>::digits10 ) << std::scientific;
    for (auto val = 1e14f * std::numbers::pi_v<float>; val < 1e15f * std::numbers::pi_v<float>; val += 1e14f)
    {
        auto [absError, relError, sinVal, sinValCustom] = evalPrecision(val);
        std::cout << "VAL: " << val << " " << "SINVAL: " << sinVal << " " << "SINVAL CUSTOM: " << sinValCustom << " " << absError << " " << relError << std::endl;
    }

       for (auto val: vVals)
    {
         auto [absError, relError, sinVal, sinValCustom] = evalPrecision(val);
        std::cout << "VAL: " << val << " " << "SINVAL: " << sinVal << " " << "SINVAL CUSTOM: " << sinValCustom << " " << absError << " " << relError << std::endl;
    }

    return 0;
}