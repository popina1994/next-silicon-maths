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

    float nextSiliconSineFP32(float x, uint32_t taylorDegreeEnd = 7);

    enum class FunctionVersion
    {
        TAYLOR_C_ORIGINAL,
        TAYLOR_CPP_OPTIMIZED,
    };

    float nextSiliconSineFP32(float x, const FunctionVersion& functionVersion);
}

#endif // FP32_CUSTOM_SINE_H