#include <gtest/gtest.h>
#include <numbers>
#include "math_custom_fun.h"

using namespace NextSilicon;

TEST(CustomSinTest, Corner) {
    EXPECT_NEAR(nextSiliconSineFP32(std::numbers::pi_v<float>, FunctionVersion::TAYLOR_C_ORIGINAL), 0, 1e-5);
    EXPECT_NEAR(nextSiliconSineFP32(-std::numbers::pi_v<float>, FunctionVersion::TAYLOR_C_ORIGINAL), 0, 1e-5);
    EXPECT_NEAR(nextSiliconSineFP32(0, FunctionVersion::TAYLOR_C_ORIGINAL), 0, 1e-5);
}

TEST(CustomSinTest, First) {
    EXPECT_EQ(1 + 1, 2);
}

TEST(SampleTest, Addition3) {
    EXPECT_EQ(1 + 1, 2);
}