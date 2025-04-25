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
        // std::cout << xPiRange << std::endl;

        if (std::abs(x) > PI_F)
        {
            xPiRange -=  std::signbit(x) * TWO_PI_F;
        }

        auto result = xPiRange;
        auto term = xPiRange;
        const auto xPiRangeSquared = xPiRange * xPiRange;
        auto sign = 1;
        std::vector<float> vFact(taylorDegreeEnd + 1, 0);
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

    static std::vector<float> computeChebyshevCoefficients(std::function<float(float)> f, uint32_t numCoeffs, float a, float b) {
        std::vector<float> vCoeffs(numCoeffs, 0.f);

        float bma = 0.5 * (b - a);
        float bpa = 0.5 * (b + a);
        // TODO: optimizations (cos(pi k...)) and theta
        for (uint32_t j = 0u; j < numCoeffs; j++) {
            float sum = 0.f;

            for (uint32_t k = 0u; k < numCoeffs; k++) {
                float leftTheta = std::numbers::pi_v<float> * (k + 0.5f) / numCoeffs;
                float rightTheta = leftTheta * j;
                float leftCos = std::cos(leftTheta);
                float rightCos = std::cos(rightTheta);
                sum += f(leftCos * bma + bpa) * rightCos;
            }
            vCoeffs[j] = sum * 2.0f / numCoeffs;
        }

        return vCoeffs;
    }

    /**
     * @brief Approximates the sine function using Chebyshev polynomials and Clenshaw's algorithm.
     *
     * This function computes an approximation of sin(x) over the interval \f$ [-\pi, \pi] \f$
     * using Chebyshev polynomials of the first kind. It first computes the Chebyshev coefficients
     * of the sine function over the specified interval, and then evaluates the series using
     * Clenshaw’s recurrence for numerical stability.
     *
     * @param x The input value in radians (expected to lie within or near [-π, π]).
     * @param chebDegreeN The number of Chebyshev terms (degree N); determines approximation accuracy.
     * @return Approximation of \f$ \sin(x) \f$ using Chebyshev expansion.
     *
     * @note
     * - Internally maps \f$ x \in [-\pi, \pi] \f$ to \f$ \xi \in [-1, 1] \f$ via affine transformation.
     * - Uses Clenshaw's algorithm to evaluate the Chebyshev series efficiently.
     * - The constant term (c₀) is halved according to Chebyshev expansion conventions.
     *
     * @see computeChebyshevCoefficients
     * @see nextSiliconSineFP32Taylor
     *
     * @example
     * float approx = nextSiliconSineFP32Chebyshev(1.0f, 8); // Approximate sin(1.0) with 8 terms
     */
    float nextSiliconSineFP32Chebyshev(float x, uint32_t chebDegreeN)
    {
        static constexpr auto PI_F = std::numbers::pi_v<float>;
        static constexpr auto TWO_PI_F = 2 * PI_F;
        auto wrappedSine = [&chebDegreeN](float x) {
            return nextSiliconSineFP32Taylor(x, chebDegreeN);
        };

        if (std::isnan(x))
        {
            throw SinNaN();
        }
        if (std::abs(x) == 0.f)
        {
            return 0;
        }
        std::cout << x << std::endl;
        auto xPiRange = fmodf(x, TWO_PI_F);

        std::cout << xPiRange << std::endl;
        if (std::abs(x) > PI_F)
        {
            xPiRange -=  std::signbit(x) * TWO_PI_F;
        }
        std::cout << xPiRange << std::endl;

        auto b = PI_F + 0.001;
        auto a = -PI_F - 0.001;
        auto y = (2.0f * xPiRange - a - b) / (b - a);
        auto y2 = 2.f * y;
        // auto chebCoeffs = computeChebyshevCoefficients(wrappedSine, chebDegreeN, a, b);
        auto chebCoeffs = computeChebyshevCoefficients(::sinf, chebDegreeN, a, b);
        // auto chebPoly = computeChebyshevPolynomial(y, chebDegreeN - 1);

        // T_0 = 1, so in the formula from the book c_0  - 1/2 c_0 is 1/2 c_0
        // chebCoeffs[0] /= 2.0f;
        // chebCoeffs[0] /= 2.0f;
        // classical
        // float sum = chebCoeffs[0];
        // for (int k = 1; k <= chebDegreeN; k++)
        // {
        //     sum += chebCoeffs[k] * chebPoly[k];
        // }
        float dMPlusTwo = 0.f;
        float dMPlusOne = 0.f;
        // Clenshaw's formula
        for (std::size_t k = chebDegreeN - 1; k > 0; k--) {
            float bCurr = y2 * dMPlusOne - dMPlusTwo + chebCoeffs[k];
            dMPlusTwo = dMPlusOne;
            dMPlusOne = bCurr;
        }

        return  y * dMPlusOne - dMPlusTwo +  chebCoeffs[0] * 0.5;
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