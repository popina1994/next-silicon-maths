#include <cmath>
#include <numbers>
#include "math_custom_fun.h"
extern "C" {
#include "fp32_custom_sine.h"
}
#include <iostream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/math/special_functions/sign.hpp>

namespace NextSilicon
{

    float nextSiliconSineFP32Taylor(float x, uint32_t degreeEnd)
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
        if (std::abs(xPiRange) > PI_F)
        {
            xPiRange -=  boost::math::sign(xPiRange) * TWO_PI_F;
        }

        auto result = xPiRange;
        auto term = xPiRange;
        const auto xPiRangeSquared = xPiRange * xPiRange;
        auto sign = 1;
        std::vector<float> vFact(degreeEnd + 1, 0);
        vFact[1] = 1;

        for (auto taylorDegree = TAYLOR_DEGREE_START;
            taylorDegree <= degreeEnd;
            taylorDegree += TAYLOR_DEGREE_NEXT_INC)
        {
            vFact[taylorDegree] = vFact[taylorDegree - 2] * (taylorDegree - 1) * taylorDegree;
        }

        for (auto taylorDegree = TAYLOR_DEGREE_START;
            taylorDegree <= degreeEnd;
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
        }

        return result;
    }

    static float optimizedFModf2Pi(float x)
    {
        using namespace boost::multiprecision;
        using Real = cpp_bin_float_100;

        static const auto TWO_PI_HIGH_PREC = 2 * boost::math::constants::pi<Real>();

        Real xExt = static_cast<Real>(x);
        auto divider = floor(xExt / TWO_PI_HIGH_PREC);
        auto remainder = xExt - divider * TWO_PI_HIGH_PREC;

        return static_cast<float>(remainder);
    }

    /**
     * @brief Computes the first polyDegree + 1 Chebyshev polynomials of the first kind at a given point.
     *
     * This function evaluates the Chebyshev polynomials ( T_0(x), T_1(x), ..., T_n(x) ) using
     * the recurrence relation:
     *
     * T_0(x) = 1, T_1(x) = x, T_n(x) = 2x * T_{n-1}(x) - T_{n-2}(x)
     *
     * @param x The evaluation point.
     * @param polyDegree The maximum degree of the Chebyshev polynomial to compute.
     * @return std::vector<float> A vector of Chebyshev polynomial values from degree 0 to polyDegree.
     *
     * @note The result vector contains polyDegree + 1 elements.
     */
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

    float nextSiliconSineFP32Chebyshev(float x, uint32_t chebDegreeN)
    {
        static constexpr auto PI_F = std::numbers::pi_v<float>;
        static constexpr auto TWO_PI_F = 2 * PI_F;
        // auto wrappedSine = [&chebDegreeN](float x) {
        //     return nextSiliconSineFP32Taylor(x, chebDegreeN);
        // };

        if (std::isnan(x))
        {
            throw SinNaN();
        }
        if (std::abs(x) == 0.f)
        {
            return 0.f;
        }
        // auto xPiRangeC = fmodf(x, TWO_PI_F);

        float xPiRange = x;
        if (std::abs(xPiRange) >= TWO_PI_F)
        {
            xPiRange = optimizedFModf2Pi(xPiRange);
        }
        if (std::abs(xPiRange) > PI_F)
        {
            xPiRange -=  boost::math::sign(xPiRange) * TWO_PI_F;
        }

        auto b = PI_F;
        auto a = -PI_F;
        auto y = (2.0f * xPiRange - a - b) / (b - a);
        auto y2 = 2.f * y;
        std::vector<float> chebCoeffs;

        if (chebDegreeN < ChebyPolyCoeffs::MAX_POLY_DEGREE)
        {
            chebCoeffs = ChebyPolyCoeffs::vvChebPoly[chebDegreeN];
        }
        else
        {
            chebCoeffs = ChebyPolyCoeffs::computeChebyshevCoefficients(static_cast<double(*)(double)>(std::sin), chebDegreeN, a, b);
        }

        float dMPlusTwo = 0.f;
        float dMPlusOne = 0.f;

        // Clenshaw's formula
        for (std::size_t k = chebDegreeN - 1; k > 0; k--) {
            float bCurr = y2 * dMPlusOne - dMPlusTwo + chebCoeffs[k];
            dMPlusTwo = dMPlusOne;
            dMPlusOne = bCurr;
        }

        return  y * dMPlusOne - dMPlusTwo +  chebCoeffs[0] * 0.5f;
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
                sineVal = nextSiliconSineFP32Taylor(x, sineArgs.degreeEnd);
                break;
            case FunctionVersion::CHEB_POLY:
                sineVal = nextSiliconSineFP32Chebyshev(x, sineArgs.degreeEnd);
                break;

        }

        return sineVal;
    }

}