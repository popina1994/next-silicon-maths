#include <cmath>
#include <numbers>
#include "math_custom_fun.h"
extern "C" {
#include "fp32_custom_sine.h"
}
#include <iostream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <functional>

namespace NextSilicon
{

    float nextSiliconSineFP32Taylor(float x, uint32_t taylorDegreeEnd)
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

    static std::vector<float> computeChebyshevPolynomial(float x, uint32_t polyDegree) {
        std::vector<float> vCoeffs(polyDegree + 1, 0.f);

        if (polyDegree >= 0)
        {
            vCoeffs[0] = 1;
        }
        if (polyDegree >= 1)
        {
            vCoeffs[1] = x;
        }

        for (uint32_t n = 2u; n <= polyDegree; n++) {
            vCoeffs[n] = 2 * x * vCoeffs[n-1]  - vCoeffs[n - 2];
        }

        return vCoeffs;
    }

    static std::vector<float> computeChebyshevCoefficients(std::function<float(float)> f, uint32_t numCoeffs) {
        std::vector<float> vCoeffs(numCoeffs, 0.f);

        // TODO: optimizations (cos(pi k...)) and theta
        for (uint32_t j = 0u; j < numCoeffs; j++) {
            float sum = 0.f;

            for (uint32_t k = 0u; k < numCoeffs; k++) {
                float theta = std::numbers::pi_v<float> * (k + 0.5f) / numCoeffs;
                float leftCos = std::cos(theta);
                float rightCos = std::cos(theta * j);
                sum += f(leftCos) * rightCos;
            }
            vCoeffs[j] = sum * 2.0f / numCoeffs;
        }

        return vCoeffs;
    }


    float nextSiliconSineFP32Chebyshev(float x, uint32_t chebDegreeN)
    {
        auto wrappedSine = [&chebDegreeN](float x) {
            return nextSiliconSineFP32Taylor(x, chebDegreeN);
        };
        auto chebCoeffs = computeChebyshevCoefficients(wrappedSine, chebDegreeN - 1);
        auto chebPoly = computeChebyshevPolynomial(x, chebDegreeN - 1);

        // T_0 = 1, so in the formula from the book c_0  - 1/2 c_0 is 1/2 c_0
        chebCoeffs[0] /= 2.0f;

        float sum = chebCoeffs[0];
        for (int k = 1; k <= chebDegreeN; k++)
        {
            sum += chebCoeffs[k] * chebPoly[k];
        }

        return sum;
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
                sineVal = nextSiliconSineFP32Taylor(x, sineArgs.taylorDegreeEnd);
                break;
            case FunctionVersion::CHEB_POLY:
                sineVal = nextSiliconSineFP32Chebyshev(x, sineArgs.taylorDegreeEnd);
                break;

        }

        return sineVal;
    }

}