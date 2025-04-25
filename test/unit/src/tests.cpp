#include <gtest/gtest.h>
#include <numbers>
#include <cmath>
#include "math_custom_fun.h"

using namespace NextSilicon;

float computeRelativeError(float goldVal, float compVal)
{
    if (std::abs(goldVal) == 0.0)
    {
        return std::abs(compVal - goldVal);
    }
    return std::abs(compVal - goldVal) / std::abs(goldVal);
}

TEST(CustomSinTest, Corner) {
    // Just to have sanity check.
    EXPECT_LE(nextSiliconSineFP32(std::numbers::pi_v<float>, FunctionVersion::TAYLOR_C_ORIGINAL), 1e-5);
    EXPECT_LE(nextSiliconSineFP32(-std::numbers::pi_v<float>, FunctionVersion::TAYLOR_C_ORIGINAL), 1e-1);
    EXPECT_LE(nextSiliconSineFP32(0, FunctionVersion::TAYLOR_C_ORIGINAL), 1e-5);
}

TEST(CustomSinTest, NaN) {
    float nanValue = std::nanf("");
    int cnt = 0;
    try {
        nextSiliconSineFP32(nanValue, FunctionVersion::TAYLOR_CPP_OPTIMIZED);
    }
    catch (const SinNaN& siNan)
    {
        cnt++;
    }

    nanValue = std::nanf("-1");
    try {
        nextSiliconSineFP32(nanValue, FunctionVersion::TAYLOR_CPP_OPTIMIZED);
    }
    catch (const SinNaN& siNan)
    {
        cnt++;
    }

    EXPECT_EQ(cnt, 2);
}

TEST(CustomSinTest, TaylorSeriesPeriodEdgeCasesMinPiDivTwoPiDivTwo) {
    std::vector<float> vVals =  {-std::numbers::pi_v<float> / 16, -std::numbers::pi_v<float> / 4,
        -std::numbers::pi_v<float> / 8 , -std::numbers::pi_v<float>/2, -0.f, 0.f,
        std::numbers::pi_v<float> / 2, std::numbers::pi_v<float> / 4,
        std::numbers::pi_v<float> / 8, std::numbers::pi_v<float> / 16};

    SineArguments sinArgs;
    sinArgs.taylorDegreeEnd = 11;
    for (auto val: vVals)
    {
        auto sinOptVal = nextSiliconSineFP32(val, FunctionVersion::TAYLOR_CPP_OPTIMIZED, sinArgs);
        auto sinGoldVal = std::sin(val);
        auto relError = computeRelativeError(sinGoldVal, sinOptVal);

        EXPECT_LE(relError, std::numeric_limits<float>::epsilon());
        // std::cout << "FIRST" sinOptVal << " " << sinGoldVal << " " << relError << std::endl;
    }
}


TEST(CustomSinTest, ChebPolyPeriodEdgeCasesMinPiDivTwoPiDivTwo) {
    std::vector<float> vVals =  {-std::numbers::pi_v<float> / 16, -std::numbers::pi_v<float> / 4,
        -std::numbers::pi_v<float> / 8 , -std::numbers::pi_v<float>/2, -0.f, 0.f,
        std::numbers::pi_v<float> / 2, std::numbers::pi_v<float> / 4,
        std::numbers::pi_v<float> / 8, std::numbers::pi_v<float> / 16};

    SineArguments sinArgs;
    sinArgs.taylorDegreeEnd = 11;
    for (auto val: vVals)
    {
        auto sinOptVal = nextSiliconSineFP32(val, FunctionVersion::CHEB_POLY, sinArgs);
        auto sinGoldVal = std::sin(val);
        auto relError = computeRelativeError(sinGoldVal, sinOptVal);
        std::cout << val << " " <<  sinOptVal << " " << sinGoldVal << " " << relError << std::endl;
        EXPECT_LE(relError, std::numeric_limits<float>::epsilon() * 10);
    }
}

TEST(CustomSinTest, PeriodEdgeCasesMinPiMinDivTwo) {
    std::vector<float> vVals =  {std::numbers::pi_v<float> / 16 + -std::numbers::pi_v<float>/2, -std::numbers::pi_v<float> / 4 + -std::numbers::pi_v<float>/2,
        -std::numbers::pi_v<float> / 8 + -std::numbers::pi_v<float>/2, -std::numbers::pi_v<float>/2 + -std::numbers::pi_v<float>/2,
        std::numbers::pi_v<float> / 2 + std::numbers::pi_v<float> / 2, std::numbers::pi_v<float> / 4 + std::numbers::pi_v<float> / 2,
        std::numbers::pi_v<float> / 8 + std::numbers::pi_v<float> / 2, std::numbers::pi_v<float> / 16 + std::numbers::pi_v<float> / 2};

    SineArguments sinArgs;
    sinArgs.taylorDegreeEnd = 15;
    for (auto val: vVals)
    {
        auto sinOptVal = nextSiliconSineFP32(val, FunctionVersion::TAYLOR_CPP_OPTIMIZED, sinArgs);
        auto sinGoldVal = std::sin(val);
        auto relError = computeRelativeError(sinGoldVal, sinOptVal);
        // std::cout << val << " " << sinOptVal << " " << sinGoldVal << " " << relError << std::endl;

        EXPECT_LE(relError, std::numeric_limits<float>::epsilon());
    }
}