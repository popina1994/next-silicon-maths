#ifndef _NEXT_SILICON_MATH_CUSTOM_SINE_H
#define _NEXT_SILICON_MATH_CUSTOM_SINE_H

#include <exception>
#include <cstdint>

namespace NextSilicon
{
    class SinNaN : public std::exception {
    public:
        // Constructor that takes a message and an error code
        SinNaN(void){}

        // Override the `what()` method to return a custom error message
        const char* what() const noexcept override {
            return "Passing Nan as an argument to a sin function";
        }
    };

    struct SineArguments
    {
        uint32_t taylorDegreeEnd = 7;
    };

    // chebyshev_t

    float nextSiliconSineFP32Taylor(float x, uint32_t taylorDegreeEnd = 7);

    float nextSiliconSineFP32Chebyshev(float x, uint32_t chebDegreeN = 7);

    enum class FunctionVersion
    {
        TAYLOR_C_ORIGINAL,
        TAYLOR_CPP_OPTIMIZED,
        CHEB_POLY
    };

        /**
     * @brief Computes sin using Chebyshev Approximation.
     *
     * This utility transforms the input x from the interval [-pi, pi] into the standard Chebyshev
     * interval [-1, 1], applies the original function, and returns the result.
     *
     * @tparam Func A callable type accepting a single float argument.
     * @param fMapped The original function defined on [-1, 1].
     * @param a The lower bound of the input domain.
     * @param b The upper bound of the input domain.
     * @return A new function that takes an input in [a, b], maps it to [-1, 1], and returns fMapped(mapped input).
     *
     * @note The caller is responsible for ensuring the input x ∈ [a, b].
     *
     * @example
     * auto chebFunc = [](float xi) { return std::cos(xi); };
     * auto wrapped = make_chebyshev_input_wrapper(chebFunc, 0.0f, 3.0f);
     * float result = wrapped(1.5f); // Internally evaluates cos(xi), where xi = 0
     */
    float nextSiliconSineFP32(float x, const FunctionVersion& functionVersion, const SineArguments& sineArgs = SineArguments{});
}

#endif // FP32_CUSTOM_SINE_H