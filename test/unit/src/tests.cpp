#include <gtest/gtest.h>
#include <numbers>
#include <cmath>
#include "math_custom_fun.h"

using namespace NextSilicon;

TEST(CustomSinTest, Corner) {
    EXPECT_NEAR(nextSiliconSineFP32(std::numbers::pi_v<float>, FunctionVersion::TAYLOR_C_ORIGINAL), 0, 1e-5);
    EXPECT_NEAR(nextSiliconSineFP32(-std::numbers::pi_v<float>, FunctionVersion::TAYLOR_C_ORIGINAL), 0, 1e-5);
    EXPECT_NEAR(nextSiliconSineFP32(0, FunctionVersion::TAYLOR_C_ORIGINAL), 0, 1e-5);
}

TEST(CustomSinTest, NaN) {
    float nanValue = std::nan("");
    int cnt = 0;
    try {
        nextSiliconSineFP32(nanValue);
    }
    catch (const SinNaN& siNan)
    {
        cnt++;
    }

    nanValue = std::nan("-1");
    try {
        nextSiliconSineFP32(nanValue);
    }
    catch (const SinNaN& siNan)
    {
        cnt++;
    }

    EXPECT_EQ(cnt, 2);
}

TEST(SampleTest, Addition3) {
    EXPECT_EQ(1 + 1, 2);
}