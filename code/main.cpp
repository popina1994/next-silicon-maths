#include "math_custom_fun.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <utility>
#include <numbers>
#include <limits>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <chrono>


namespace po = boost::program_options;
using namespace NextSilicon;

std::tuple<float, float, float, float> evalPrecision(float val, const FunctionVersion& funcVer,
    const SineArguments& sineArgs)
{
    auto sinValCustom = nextSiliconSineFP32(val, funcVer, sineArgs);
    auto sinVal = ::sinf(val);
    auto absError = abs(sinVal - sinValCustom);
    auto relError = abs(absError) / abs(sinVal);
    return {(float)absError, (float)relError, sinValCustom, (float)sinVal};
}

static void testAccuracyRange(float startRange, float endRange, float incRange,
    const FunctionVersion& functVersion,
    const SineArguments& sineArgs,
    const std::string& outputPath)
{
    std::ofstream fOut(outputPath + ".txt");
    std::cout << "Running accuracy experiments:" << outputPath << std::endl;
    fOut << std::setprecision(std::numeric_limits<float>::digits10 ) << std::scientific;
    for (auto val = startRange; val <= endRange; val += incRange)
    {
        auto [absError, relError, sinVal, sinValCustom] = evalPrecision(val, functVersion, sineArgs);
        fOut << "VAL: " << val << " " << "SINVAL: " << sinVal << " " << "SINVAL CUSTOM: " << sinValCustom << " ABS: " << absError << " REL: " << relError << std::endl;
    }
}

static void testPerformanceRange(float startRange, float endRange, float incRange,
    const FunctionVersion& funcVer,
    const SineArguments& sineArgs,
    const std::string& outputPath)
{
    using namespace std::chrono;
    static constexpr auto NUM_RUNS = 1000;
    std::ofstream fOut(outputPath + ".txt");
    std::cout << "Running performance experiments:" << outputPath << std::endl;
    for (auto val = startRange; val <= endRange; val += incRange)
    {
        auto startTime = high_resolution_clock::now();
        for (int idx = 0; idx < NUM_RUNS; idx++)
        {
            auto sinVal = nextSiliconSineFP32(val, funcVer, sineArgs);
        }
        auto endTime = high_resolution_clock::now();
        auto elapsedTime = duration_cast<nanoseconds>(endTime - startTime);

        fOut << "VAL: " << val << "Time taken: " << elapsedTime.count() << " nanoseconds" << std::endl;
    }
}

static void evalTest(bool testPiRange, bool testLargeNumbers, bool testTwoPiRange,
    bool testZeroZeroZeroOneRange,
    const FunctionVersion& functVersion, const SineArguments& sineArgs,
    const std::string& outputPath, bool testAccuracy)
{
    if (testPiRange)
    {
        float piRangeNumStart = -std::numbers::pi_v<float>;
        float piRangeNumEnd = std::numbers::pi_v<float>;
        float piRangeNumInc = 0.001f;
        auto outPathUpdate = outputPath + "pi_inc";
        if (testAccuracy)
        {
            testAccuracyRange(piRangeNumStart, piRangeNumEnd, piRangeNumInc, functVersion, sineArgs, outPathUpdate);
        }
        else
        {
            testPerformanceRange(piRangeNumStart, piRangeNumEnd, piRangeNumInc, functVersion, sineArgs, outPathUpdate);
        }
    }

    if (testLargeNumbers)
    {
        float largeNumStart = 1e14f * std::numbers::pi_v<float>;
        float largeNumEnd = 1e15f * std::numbers::pi_v<float>;
        float largeNumInc = 1e14f;
        auto outPathUpdate = outputPath + "large_num_inc";
        if (testAccuracy)
        {
            testAccuracyRange(largeNumStart, largeNumEnd, largeNumInc, functVersion, sineArgs, outPathUpdate);
        }
        else
        {
            testPerformanceRange(largeNumStart, largeNumEnd, largeNumInc, functVersion, sineArgs, outPathUpdate);
        }

    }

    if (testTwoPiRange)
    {
        float twoPiRangeNumStart = -2 * std::numbers::pi_v<float>;
        float twoPiRangeNumEnd = 2 * std::numbers::pi_v<float>;
        float twoPiRangeNumInc = 0.001f;
        auto outPathUpdate = outputPath + "two_pi_num_inc";
        if (testAccuracy)
        {
            testAccuracyRange(twoPiRangeNumStart, twoPiRangeNumEnd, twoPiRangeNumInc, functVersion, sineArgs, outPathUpdate);
        }
        else
        {
            testPerformanceRange(twoPiRangeNumStart, twoPiRangeNumEnd, twoPiRangeNumInc, functVersion, sineArgs, outPathUpdate);
        }

    }

    if (testTwoPiRange)
    {
        float twoPiRangeNumStart = -4 * std::numbers::pi_v<float>;
        float twoPiRangeNumEnd = 4 * std::numbers::pi_v<float>;
        float twoPiRangeNumInc = 0.001f;
        auto outPathUpdate = outputPath + "four_pi_num_inc";
        if (testAccuracy)
        {
            testAccuracyRange(twoPiRangeNumStart, twoPiRangeNumEnd, twoPiRangeNumInc, functVersion, sineArgs, outPathUpdate);
        }
        else
        {
            testPerformanceRange(twoPiRangeNumStart, twoPiRangeNumEnd, twoPiRangeNumInc, functVersion, sineArgs, outPathUpdate);
        }

    }

    if (testZeroZeroZeroOneRange)
    {
        float twoPiRangeNumStart = -0.0001f;
        float twoPiRangeNumEnd = 0.0001f;
        float twoPiRangeNumInc = 0.000001f;
        auto outPathUpdate = outputPath + "zero_zero_zero_one_num_inc";
        if (testAccuracy)
        {
            testAccuracyRange(twoPiRangeNumStart, twoPiRangeNumEnd, twoPiRangeNumInc, functVersion, sineArgs, outPathUpdate);
        }
        else
        {
            testPerformanceRange(twoPiRangeNumStart, twoPiRangeNumEnd, twoPiRangeNumInc, functVersion, sineArgs, outPathUpdate);
        }

    }
}


void parseArgs(int argc, char* argv[], bool& testAccuracyFlag, bool& testPerformanceFlag, std::string& outputPath)
{
    try
    {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "Produce help message")
            ("accuracy", po::bool_switch(&testAccuracyFlag)->default_value(false), "Enable accuracy mode")
            ("performance", po::bool_switch(&testPerformanceFlag)->default_value(false), "Enable performance mode")
            ("output", po::value<std::string>(&outputPath)->required(), "Output file path")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return;
        }

        po::notify(vm);

        std::cout << "Accuracy: " << std::boolalpha << testAccuracyFlag << "\n";
        std::cout << "Performance: " << std::boolalpha << testPerformanceFlag << "\n";
        std::cout << "Output path: " << outputPath << "\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return;
    }

}

void evalExperiments(bool testAccuracyFlag, bool testPerformanceFlag, const std::string& outputPath)
{
    std::vector<FunctionVersion> vFunVers = {FunctionVersion::TAYLOR_C_ORIGINAL, FunctionVersion::TAYLOR_CPP_OPTIMIZED, FunctionVersion::CHEB_POLY};
    std::vector<uint32_t> vDegreeEnd = {7, 11, 15};
     for (auto funVer: vFunVers)
    {
        for (auto degreeEnd: vDegreeEnd)
        {
            SineArguments sineArg;
            sineArg.degreeEnd = degreeEnd;
            if (testAccuracyFlag)
            {
                auto outPathUpdate = outputPath + "/accuracy/" + funcVersiontoString(funVer) + "_" + std::to_string(degreeEnd);
                evalTest(true, true, true, true, funVer, sineArg, outPathUpdate, true);
            }

            if (testPerformanceFlag)
            {
                auto outPathUpdate = outputPath + "/performance/" + funcVersiontoString(funVer) + "_" + std::to_string(degreeEnd);
                evalTest(true, true, true, true, funVer, sineArg, outPathUpdate, false);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    std::cout << std::setprecision(std::numeric_limits<float>::digits10 ) << std::scientific;

    bool testAccuracyFlag = true;
    bool testPerformanceFlag = false;
    std::string outputPath;

    parseArgs(argc, argv, testAccuracyFlag, testPerformanceFlag, outputPath);
    evalExperiments(testAccuracyFlag, testPerformanceFlag, outputPath);

    return 0;
}