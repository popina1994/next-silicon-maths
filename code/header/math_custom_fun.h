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

    float nextSiliconSineFP32(float x, const FunctionVersion& functionVersion, const SineArguments& sineArgs = SineArguments{});
}

#endif // FP32_CUSTOM_SINE_H