#include <cmath>
#include <numbers>
#include "math_custom_fun.h"
extern "C" {
#include "fp32_custom_sine.h"
}
#include <iostream>

namespace NextSilicon
{

    float nextSiliconSineFP32(float x, uint32_t taylorDegreeEnd)
    {
        static constexpr auto PI_F = std::numbers::pi_v<float>;
        static constexpr auto TWO_PI_F = 2 * PI_F;
        static constexpr auto TAYLOR_DEGREE_START = 3u;
        static constexpr auto TAYLOR_DEGREE_NEXT_INC = 2u;
        if (std::isnan(x))
        {
            throw SinNaN();
        }
        if (std::abs(x) == 0.f)
        {
            return 0;
        }

        auto xPiRange = fmodf(x, TWO_PI_F);
        std::cout << "X IN range" << xPiRange << std::endl;

        if (std::abs(x) > PI_F)
        {
            xPiRange -=  std::signbit(x) * TWO_PI_F;
        }

        auto result = xPiRange;
        auto term = xPiRange;
        const auto xPiRangeSquared = xPiRange * xPiRange;
        auto sign = 1;

        for (auto taylorDegree = TAYLOR_DEGREE_START;
            taylorDegree <= taylorDegreeEnd;
            taylorDegree += TAYLOR_DEGREE_NEXT_INC)
        {
            sign = -sign;
            term = term * xPiRangeSquared;
            term = term / ((taylorDegree - 1) * taylorDegree);
            result += sign * term;
            // std::cout << taylorDegree << " " << result << std::endl;
        }

        return result;
    }

    float nextSiliconSineFP32(float x, const FunctionVersion& functionVersion)
    {
        auto sineVal = std::nanf("");
        switch(functionVersion)
        {
            case FunctionVersion::TAYLOR_C_ORIGINAL:
                sineVal = fp32_custom_sine(x);
                break;
            case FunctionVersion::TAYLOR_CPP_OPTIMIZED:
                sineVal = nextSiliconSineFP32(x, 7);
                break;

        }

        return sineVal;
    }

}