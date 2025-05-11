#ifndef _NEXT_SILICON_MATH_CUSTOM_SINE_H
#define _NEXT_SILICON_MATH_CUSTOM_SINE_H

#include <exception>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>
#include <functional>

namespace NextSilicon
{
    class SinNaN : public std::exception {
    public:
        SinNaN(void){}

        const char* what() const noexcept override {
            return "Passing Nan as an argument to a sin function";
        }
    };

    struct SineArguments
    {
        uint32_t degreeEnd = 7;
    };

    struct ChebyPolyCoeffs
    {
        static constexpr uint32_t MAX_POLY_DEGREE = 20;
        using ChebCoefType = std::vector<std::vector<float>>;
         /**
         * @brief Computes Chebyshev polynomial approximation coefficients for a given function.
         *
         * This function calculates the Chebyshev coefficients for approximating a given function @p f
         * over the interval [a, b] using the Chebyshev polynomials of the first kind.
         *
         * @param f The function to approximate, passed as a std::function<float(float)>.
         * @param numCoeffs The number of Chebyshev coefficients to compute.
         * @param a The lower bound of the approximation interval.
         * @param b The upper bound of the approximation interval.
         * @return std::vector<float> A vector of Chebyshev coefficients of size @p numCoeffs
         *
         * @note The function evaluates @p f at Chebyshev nodes and computes the coefficients using
         *       the discrete orthogonality of Chebyshev polynomials.
         * @note time complexity O(numCoeffs^2), space complexity O(numCoeffs)
         * @todo Optimize cosine term computations by precomputing angles or using recurrence.
         *
         * @see https://en.wikipedia.org/wiki/Chebyshev_polynomials for more details on Chebyshev approximation.
         */
        static std::vector<float> computeChebyshevCoefficients(std::function<float(float)> f, uint32_t numCoeffs, float a, float b) {
            std::vector<float> vCoeffs(numCoeffs, 0.f);

            float bma = 0.5f * (b - a);
            float bpa = 0.5f * (b + a);
            for (uint32_t j = 0u; j < numCoeffs; j++) {
                float sum = 0.f;

                for (uint32_t k = 0u; k < numCoeffs; k++) {
                    float leftTheta = std::numbers::pi_v<float> * (k + 0.5f) / numCoeffs;
                    float rightTheta = leftTheta * j;
                    float leftCos = ::cosf(leftTheta);
                    float rightCos = ::cosf(rightTheta);
                    sum += f(leftCos * bma + bpa) * rightCos;
                }
                vCoeffs[j] = sum * 2.0f / numCoeffs;
            }

            return vCoeffs;
        }

        /**
         * @brief Precomputes Chebyshev polynomial coefficients for the sine function over the interval [-pi, pi].
         *
         * This function iteratively computes Chebyshev coefficients for polynomial degrees from 2 up to
         * MAX_POLY_DEGREE - 1. It uses the sine function (::sinf) as the target function to approximate,
         * and stores the resulting coefficients in a fixed-size 2D array.
         *
         * @return ChebCoefType A 2D array containing Chebyshev coefficients for each polynomial degree.
         *                      The outer index corresponds to the polynomial degree.
         */
        static ChebCoefType computeAllChebCoefSyn()
        {
            ChebyPolyCoeffs::ChebCoefType result(MAX_POLY_DEGREE);
            constexpr auto PI_F = std::numbers::pi_v<float>;
            constexpr auto a = -0.0001f;
            constexpr auto b = 0.0001f;

            for (uint32_t numCoeffs = 2; numCoeffs < MAX_POLY_DEGREE; numCoeffs++)
            {
                result[numCoeffs] = computeChebyshevCoefficients(static_cast<float(*)(float)>(::sinf), numCoeffs, a, b);
            }

            return result;
        }

        static const ChebCoefType vvChebPoly;
    };

    inline const ChebyPolyCoeffs::ChebCoefType ChebyPolyCoeffs::vvChebPoly = ChebyPolyCoeffs::computeAllChebCoefSyn();


    /**
     * @brief Approximates the sine of a floating-point angle using a Taylor series.
     *
     * This function computes an approximation of the sine function for a given angle x
     * in radians using a Taylor series expansion up to a specified degree.
     * The input is first reduced to the range [-pi, pi] for better convergence.
     *
     * @param x The angle in radians (single-precision float).
     * @param degreeEnd The maximum degree (odd number) to use in the Taylor series expansion.
     *                        Must be ≥ 3 and should be odd for sine approximation.
     * @return float Approximate sine of the input angle.
     *
     * @throws SinNaN If the input `x` is NaN or -NaN.
     *
     * @note The function uses precomputed factorials to improve performance and assumes
     *       that the caller provides a valid degree (≥ 3 and odd). Accuracy increases
     *       with higher `degreeEnd` but may introduce more FP32 rounding error.
     *
     * @see std::sinf for comparison with the standard library sine function.
     */
    float nextSiliconSineFP32Taylor(float x, uint32_t degreeEnd = 7);

    /**
     * @brief Approximates the sine function using Chebyshev polynomials and Clenshaw's algorithm.
     *
     * This function computes an approximation of sin(x) over the interval \f$ [-\pi, \pi] \f$
     * using Chebyshev polynomials of the first kind. First we map the number to the interval [-pi, pi] using fmodf first.
     * Then we computes the Chebyshev coefficients of the sine function over the interval [-pi ,pi], and then evaluates the series using
     * Clenshaw’s recurrence for numerical stability.
     *
     * @param x The input value in radians
     * @param chebDegreeN The number of Chebyshev terms (degree N); determines approximation accuracy.
     * @return Approximation of \f$ \sin(x) \f$ using Chebyshev expansion.
     *
     * @note
     * - Internally maps \f$ x \in [-\pi, \pi] \f$ to \f$ \xi \in [-1, 1] \f$ via affine transformation.
     * - Uses Clenshaw's algorithm to evaluate the Chebyshev series efficiently.
     * - The constant term (c_0) is halved according to Chebyshev expansion conventions.
     * - The time complexity is O(chebDegree) if chebDegree is less than ChebyPolyCoeffs::MAX_POLY_DEGREE
     *  since in this case the coefficients are precomputed, Otherwise it is O(chebDegree^2).
     *
     * @see computeChebyshevCoefficients
     * @see nextSiliconSineFP32Taylor
     *
     * @example
     * float approx = nextSiliconSineFP32Chebyshev(1.0f, 8); // Approximate sin(1.0) with 8 terms
     */
    float nextSiliconSineFP32Chebyshev(float x, uint32_t chebDegreeN = 7);

    enum class FunctionVersion
    {
        TAYLOR_C_ORIGINAL,
        TAYLOR_CPP_OPTIMIZED,
        CHEB_POLY
    };

    inline std::string funcVersiontoString(const FunctionVersion& version)
    {
        switch (version)
        {
            case FunctionVersion::TAYLOR_C_ORIGINAL: return "TAYLOR_C_ORIGINAL";
            case FunctionVersion::TAYLOR_CPP_OPTIMIZED: return "TAYLOR_CPP_OPTIMIZED";
            case FunctionVersion::CHEB_POLY: return "CHEB_POLY";
            default: throw std::invalid_argument("Unknown FunctionVersion");
        }
    }

     /**
     * @brief Dispatches and computes the sine approximation for a given input using a selected method.
     *
     * This function chooses between multiple sine approximation algorithms (e.g., Taylor series,
     * Chebyshev polynomial) based on the specified functionVersion, and evaluates the sine
     * of @p x using the corresponding method.
     *
     * @param x The input value (angle in radians).
     * @param functionVersion Enum indicating the algorithm to use (e.g., Taylor original, optimized Taylor original, Chebyshev).
     * @param sineArgs Struct containing method-specific parameters (e.g., polynomial degree).
     * @return float The approximated sine of @p x based on the selected method.
     *
     * @see nextSiliconSineFP32Taylor, fp32_custom_sine, nextSiliconSineFP32Chebyshev
     */
    float nextSiliconSineFP32(float x, const FunctionVersion& functionVersion, const SineArguments& sineArgs = SineArguments{});
}

#endif