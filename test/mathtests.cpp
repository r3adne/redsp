//
// Created by ariadne lewis-towbes on 5/3/23.
//
#include "../source/internal/remath.h"
#include "mathtests.h"

using namespace juce;
using namespace redsp;


void MathTest::runTest()
{
    beginTest("Approximations_double");
    auto rand = getRandom();

    juce::NormalisableRange<double> tanrange{-remath::halfpi<double>() + 0.05, remath::halfpi<double>() - 0.05};

    juce::NormalisableRange<double> sinrange{-remath::pi<double>(), remath::pi<double>()};

    for (int i = 0; i < 100000; ++i)
    {
        auto r = rand.nextDouble();

        auto rwithintan = tanrange.convertFrom0to1(r);
        expectWithinAbsoluteError(remath::tan<double>(rwithintan), std::tan(rwithintan), 0.0000005);
        expectWithinAbsoluteError(remath::tan_fast<double>(rwithintan), std::tan(rwithintan), 0.05);
        expectWithinAbsoluteError(remath::tan_faster<double>(rwithintan), std::tan(rwithintan), 0.1);

        auto rwithinsin = sinrange.convertFrom0to1(r);
        expectWithinAbsoluteError(remath::sin<double>(rwithinsin), std::sin(rwithinsin), 0.0000005);
        expectWithinAbsoluteError(remath::sin_fast<double>(rwithinsin), std::sin(rwithinsin), 0.05);
        expectWithinAbsoluteError(remath::sin_faster<double>(rwithinsin), std::sin(rwithinsin), 0.1);

        expectWithinAbsoluteError(remath::cos<double>(rwithinsin), std::cos(rwithinsin), 0.0000005);
        expectWithinAbsoluteError(remath::cos_fast<double>(rwithinsin), std::cos(rwithinsin), 0.005);
        expectWithinAbsoluteError(remath::cos_faster<double>(rwithinsin), std::cos(rwithinsin), 0.1);
        expectWithinAbsoluteError(remath::cos_fastest<double>(rwithinsin), std::cos(rwithinsin), 2.0); // lmao

        auto rwide = ((r * 2.0) - 1.0) * 100000.0;

        expectWithinAbsoluteError(remath::tanh<double>(rwide), std::tanh(rwide), 0.0000005);
        expectWithinAbsoluteError(remath::tanh_fast<double>(rwide), std::tanh(rwide), 0.005);
        expectWithinAbsoluteError(remath::tanh_faster<double>(rwide), std::tanh(rwide), 0.05);

        rwide = abs(rwide);
        expectWithinAbsoluteError(remath::log2<double>(rwide), std::log2(rwide), 0.0000005);
        expectWithinAbsoluteError(remath::log2_fast<double>(rwide), std::log2(rwide), 0.005);

        expectWithinAbsoluteError(remath::sqrt<double>(rwide), std::sqrt(rwide), 0.0000005);
    }

//    beginTest("Approximations_float");
//
//    for (int i = 0; i < 100000; ++i)
//    {
//        auto r = fmod(rand.nextFloat(), remath::pi<float>()) * 100;
//
//
//        if (abs(tan(r)) < 50)
//        {
//            expectWithinAbsoluteError(static_cast<double>(remath::tan<float>(r)), static_cast<double>(std::tan(r)), 0.0000005);
//            expectWithinAbsoluteError(static_cast<double>(remath::tan_fast_complete<float>(r)), static_cast<double>(std::tan(r)), 0.05);
//            expectWithinAbsoluteError(static_cast<double>(remath::tan_faster_complete<float>(r)), static_cast<double>(std::tan(r)), 0.1);
//        }
//
//        expectWithinAbsoluteError(static_cast<double>(remath::sin<float>(r)), static_cast<double>(std::sin(r)), 0.0000005);
//        expectWithinAbsoluteError(static_cast<double>(remath::sin_fast_complete<float>(r)), static_cast<double>(std::sin(r)), 0.05);
//        expectWithinAbsoluteError(static_cast<double>(remath::sin_faster_complete<float>(r)), static_cast<double>(std::sin(r)), 0.1);
//
//        expectWithinAbsoluteError(static_cast<double>(remath::cos<float>(r)), static_cast<double>(std::cos(r)), 0.0000005);
//        expectWithinAbsoluteError(static_cast<double>(remath::cos_fast<float>(r)), static_cast<double>(std::cos(r)), 0.05);
//        expectWithinAbsoluteError(static_cast<double>(remath::cos_faster<float>(r)), static_cast<double>(std::cos(r)), 0.1);
//
//        expectWithinAbsoluteError(static_cast<double>(remath::tanh<float>(r)), static_cast<double>(std::tanh(r)), 0.0000005);
//        expectWithinAbsoluteError(static_cast<double>(remath::tanh_fast<float>(r)), static_cast<double>(std::tanh(r)), 0.005);
//        expectWithinAbsoluteError(static_cast<double>(remath::tanh_faster<float>(r)), static_cast<double>(std::tanh(r)), 0.05);
//
//        r = abs(r);
//
//        expectWithinAbsoluteError(static_cast<double>(remath::log2<float>(r)), static_cast<double>(std::log2(r)), 0.0000005);
//        expectWithinAbsoluteError(static_cast<double>(remath::log2_fast<float>(r)), static_cast<double>(std::log2(r)), 0.005);
//
//        expectWithinAbsoluteError(static_cast<double>(remath::sqrt<float>(r)), static_cast<double>(std::sqrt(r)), 0.0000005);
//    }
//
//    beginTest("Approximations_longdouble");
//
//    for (int i = 0; i < 100000; ++i)
//    {
//        auto r = fmod(static_cast<long double>(rand.nextDouble()), remath::pi<long double>()) * 100;
//
//        if (abs(tan(r)) < 50)
//        {
//            expectWithinAbsoluteError(static_cast<double>(remath::tan<long double>(r)), static_cast<double>(std::tan(r)), 0.0000005);
//            expectWithinAbsoluteError(static_cast<double>(remath::tan_fast_complete<long double>(r)), static_cast<double>(std::tan(r)), 0.05);
//            expectWithinAbsoluteError(static_cast<double>(remath::tan_faster_complete<long double>(r)), static_cast<double>(std::tan(r)), 0.1);
//        }
//
//        expectWithinAbsoluteError(static_cast<double>(remath::sin<long double>(r)), static_cast<double>(std::sin(r)), 0.0000005);
//        expectWithinAbsoluteError(static_cast<double>(remath::sin_fast_complete<long double>(r)), static_cast<double>(std::sin(r)), 0.005);
//        expectWithinAbsoluteError(static_cast<double>(remath::sin_faster_complete<long double>(r)), static_cast<double>(std::sin(r)), 0.1);
//
//        expectWithinAbsoluteError(static_cast<double>(remath::cos<long double>(r)), static_cast<double>(std::cos(r)), 0.0000005);
//        expectWithinAbsoluteError(static_cast<double>(remath::cos_fast<long double>(r)), static_cast<double>(std::cos(r)), 0.005);
//        expectWithinAbsoluteError(static_cast<double>(remath::cos_faster<long double>(r)), static_cast<double>(std::cos(r)), 0.1);
//
//        expectWithinAbsoluteError(static_cast<double>(remath::tanh<long double>(r)), static_cast<double>(std::tanh(r)), 0.0000005);
//        expectWithinAbsoluteError(static_cast<double>(remath::tanh_fast<long double>(r)), static_cast<double>(std::tanh(r)), 0.005);
//        expectWithinAbsoluteError(static_cast<double>(remath::tanh_faster<long double>(r)), static_cast<double>(std::tanh(r)), 0.05);
//
//        r = abs(r);
//
//        expectWithinAbsoluteError(static_cast<double>(remath::log2<long double>(r)), static_cast<double>(std::log2(r)), 0.0000005);
//        expectWithinAbsoluteError(static_cast<double>(remath::log2_fast<long double>(r)), static_cast<double>(std::log2(r)), 0.005);
//
//        expectWithinAbsoluteError(static_cast<double>(remath::sqrt<long double>(r)), static_cast<double>(std::sqrt(r)), 0.0000005);
//    }

    // this is better done in an analytical setting.
    {
//        beginTest("tan_limit_tests");
//
//        auto hpiminusepsilonf = remath::halfpi<float>() - std::numeric_limits<float>::epsilon();
//        auto neghpiplusepsilonf = -1.f * hpiminusepsilonf;
//        auto ftanlimpos = remath::tan_fast<float>(hpiminusepsilonf);
//        auto ftanlimnegs = remath::tan_fast<float>(neghpiplusepsilonf);
//
//        auto fcorpos = remath::tan<float>(hpiminusepsilonf);
//        auto fcorneg = remath::tan<float>(neghpiplusepsilonf);
//
//        auto poserr = abs(ftanlimpos - fcorpos);
//        auto negerr = abs(ftanlimnegs - fcorneg);
//
//        DBG(poserr);
//        DBG(negerr);
    }


}