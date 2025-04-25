#ifndef _NEXT_SILICON_MATH_CUSTOM_SINE_H
#define _NEXT_SILICON_MATH_CUSTOM_SINE_H

namespace NextSilicon
{
    float nextSiliconSineFP32(float x, int taylorDegreeEnd = 7);

        enum class FunctionVersion
    {
        TAYLOR_C_ORIGINAL,
        TAYLOR_CPP_OPTIMIZED,
    };

    float nextSiliconSineFP32(float x, const FunctionVersion& functionVersion);
}

#endif // FP32_CUSTOM_SINE_H