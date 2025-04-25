#include <cmath>
#include <numbers>
#include "math_custom_fun.h"
extern "C" {
#include "fp32_custom_sine.h"
}
#include <iostream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

namespace NextSilicon
{

    float nextSiliconSineFP32(float x, uint32_t taylorDegreeEnd)
    {
        using namespace boost::multiprecision;
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
        // std::cout << "X IN range" << xPiRange << std::endl;

        if (std::abs(x) > PI_F)
        {
            xPiRange -=  std::signbit(x) * TWO_PI_F;
        }

        auto result = xPiRange;
        auto term = xPiRange;
        const auto xPiRangeSquared = xPiRange * xPiRange;
        auto sign = 1;
        std::vector<cpp_int> vFact(taylorDegreeEnd + 1, 0);
        vFact[1] = 1;

        for (auto taylorDegree = TAYLOR_DEGREE_START;
            taylorDegree <= taylorDegreeEnd;
            taylorDegree += TAYLOR_DEGREE_NEXT_INC)
        {
            vFact[taylorDegree] = vFact[taylorDegree - 2] * (taylorDegree - 1) * taylorDegree;
        }

        for (auto taylorDegree = TAYLOR_DEGREE_START;
            taylorDegree <= taylorDegreeEnd;
            taylorDegree += TAYLOR_DEGREE_NEXT_INC)
        {
            sign = -sign;
            term = term * xPiRangeSquared;
            auto termAdd = term / float(vFact[taylorDegree]);
            if (sign == 1)
            {
                result += termAdd;
            }
            else
            {
                result -= termAdd;
            }

            // std::cout << taylorDegree << " " << result << std::endl;
        }

        return result;
    }

    float nextSiliconSineFP32(float x, const FunctionVersion& functionVersion, const SineArguments& sineArgs)
    {
        auto sineVal = std::nanf("");
        switch(functionVersion)
        {
            case FunctionVersion::TAYLOR_C_ORIGINAL:
                sineVal = fp32_custom_sine(x);
                break;
            case FunctionVersion::TAYLOR_CPP_OPTIMIZED:
                sineVal = nextSiliconSineFP32(x, sineArgs.taylorDegreeEnd);
                break;
        }

        return sineVal;
    }

}