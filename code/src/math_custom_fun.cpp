#include <cmath>
#include <numbers>
#include "math_custom_fun.h"

namespace NextSilicon
{
    float nextSiliconSineFP32(float x, int taylorDegreeEnd)
    {
        static constexpr auto PI_F = std::numbers::pi_v<float>;
        static constexpr auto TWO_PI_F = 2 * PI_F;
        static constexpr auto TAYLOR_DEGREE_START = 3;
        static constexpr auto TAYLOR_DEGREE_NEXT_INC = 3;

        auto xPiRange = fmodf(x, TWO_PI_F);

        if (abs(x) > PI_F)
        {
            xPiRange -=  std::signbit(x) * TWO_PI_F;
        }

        auto result = xPiRange;
        auto term = xPiRange;
        auto xPiRangeSquared = xPiRange * xPiRange;
        auto sign = 1;

        for (auto taylorDegree = TAYLOR_DEGREE_START;
            taylorDegree <= taylorDegreeEnd;
            taylorDegree += TAYLOR_DEGREE_NEXT_INC)
        {
            sign = -sign;
            term = term * xPiRangeSquared;
            term = term / ((taylorDegree - 1) * taylorDegree);
            result += sign * term;
        }

        return result;
    }

}