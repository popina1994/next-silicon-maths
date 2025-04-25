#ifndef _NEXT_SILICON_MATH_CUSTOM_SINE_H
#define _NEXT_SILICON_MATH_CUSTOM_SINE_H

#include <exception>
#include <cstdint>

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
        uint32_t taylorDegreeEnd = 7;
    };

    /**
     * @brief Approximates the sine of a floating-point angle using a Taylor series.
     *
     * This function computes an approximation of the sine function for a given angle x
     * in radians using a Taylor series expansion up to a specified degree.
     * The input is first reduced to the range [-pi, pi] for better convergence.
     *
     * @param x The angle in radians (single-precision float).
     * @param taylorDegreeEnd The maximum degree (odd number) to use in the Taylor series expansion.
     *                        Must be ≥ 3 and should be odd for sine approximation.
     * @return float Approximate sine of the input angle.
     *
     * @throws SinNaN If the input `x` is NaN or -NaN.
     *
     * @note The function uses precomputed factorials to improve performance and assumes
     *       that the caller provides a valid degree (≥ 3 and odd). Accuracy increases
     *       with higher `taylorDegreeEnd` but may introduce more FP32 rounding error.
     *
     * @see std::sinf for comparison with the standard library sine function.
     */
    float nextSiliconSineFP32Taylor(float x, uint32_t taylorDegreeEnd = 7);

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