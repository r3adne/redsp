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

    {
        juce::NormalisableRange<double> tanrange{-remath::halfpi < double > () + 0.05,
                                                 remath::halfpi < double > () - 0.05};
        juce::NormalisableRange<double> sinrange{-remath::pi < double > (), remath::pi < double > ()};

        for (int i = 0; i < 100000; ++i) {
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
    }

    {
        juce::NormalisableRange<float> tanrange{-remath::halfpi < float > () + 0.05f,
                                                remath::halfpi < float > () - 0.05f};
        juce::NormalisableRange<float> sinrange{-remath::pi < float > (), remath::pi < float > ()};

        beginTest("Approximations_float");
        for (int i = 0; i < 100000; ++i) {
            auto r = rand.nextFloat();

            auto rwithintan = tanrange.convertFrom0to1(r);
            expectWithinAbsoluteError(static_cast<double>(remath::tan<float>(rwithintan)), static_cast<double>(std::tan(rwithintan)), 0.0000005);
            expectWithinAbsoluteError(static_cast<double>(remath::tan_fast<float>(rwithintan)), static_cast<double>(std::tan(rwithintan)), 0.05);
            expectWithinAbsoluteError(static_cast<double>(remath::tan_faster<float>(rwithintan)), static_cast<double>(std::tan(rwithintan)), 0.1);

            auto rwithinsin = sinrange.convertFrom0to1(r);
            expectWithinAbsoluteError(static_cast<double>(remath::sin<float>(rwithinsin)), static_cast<double>(std::sin(rwithinsin)), 0.0000005);
            expectWithinAbsoluteError(static_cast<double>(remath::sin_fast<float>(rwithinsin)), static_cast<double>(std::sin(rwithinsin)), 0.05);
            expectWithinAbsoluteError(static_cast<double>(remath::sin_faster<float>(rwithinsin)), static_cast<double>(std::sin(rwithinsin)), 0.1);

            expectWithinAbsoluteError(static_cast<double>(remath::cos<float>(rwithinsin)), static_cast<double>(std::cos(rwithinsin)), 0.0000005);
            expectWithinAbsoluteError(static_cast<double>(remath::cos_fast<float>(rwithinsin)), static_cast<double>(std::cos(rwithinsin)), 0.005);
            expectWithinAbsoluteError(static_cast<double>(remath::cos_faster<float>(rwithinsin)), static_cast<double>(std::cos(rwithinsin)), 0.1);
            expectWithinAbsoluteError(static_cast<double>(remath::cos_fastest<float>(rwithinsin)), static_cast<double>(std::cos(rwithinsin)), 2.0); // lmao

            auto rwide = ((r * 2.0) - 1.0) * 100000.0;
            expectWithinAbsoluteError(static_cast<double>(remath::tanh<float>(rwide)), static_cast<double>(std::tanh(rwide)), 0.0000005);
            expectWithinAbsoluteError(static_cast<double>(remath::tanh_fast<float>(rwide)), static_cast<double>(std::tanh(rwide)), 0.005);
            expectWithinAbsoluteError(static_cast<double>(remath::tanh_faster<float>(rwide)), static_cast<double>(std::tanh(rwide)), 0.05);

            rwide = abs(rwide);
            expectWithinAbsoluteError(static_cast<double>(remath::log2<float>(rwide)), static_cast<double>(std::log2(rwide)), 0.0005);
            expectWithinAbsoluteError(static_cast<double>(remath::log2_fast<float>(rwide)), static_cast<double>(std::log2(rwide)), 0.005);

            expectWithinAbsoluteError(static_cast<double>(remath::sqrt<double>(rwide)), static_cast<double>(std::sqrt(rwide)), 0.0005);
        }
    }

    {
        juce::NormalisableRange<long double> tanrange{-remath::halfpi < long double > () + 0.05f,
                                                remath::halfpi < long double > () - 0.05f};
        juce::NormalisableRange<long double> sinrange{-remath::pi < long double > (), remath::pi < long double > ()};

        beginTest("Approximations_longdouble");
        for (int i = 0; i < 100000; ++i)
        {
            auto r = static_cast<long double>(rand.nextDouble());

            auto rwithintan = tanrange.convertFrom0to1(r);
            expectWithinAbsoluteError(static_cast<double>(remath::tan<long double>(rwithintan)), static_cast<double>(std::tan(rwithintan)), 0.0000005);
            expectWithinAbsoluteError(static_cast<double>(remath::tan_fast<long double>(rwithintan)), static_cast<double>(std::tan(rwithintan)), 0.05);
            expectWithinAbsoluteError(static_cast<double>(remath::tan_faster<long double>(rwithintan)), static_cast<double>(std::tan(rwithintan)), 0.1);

            auto rwithinsin = sinrange.convertFrom0to1(r);
            expectWithinAbsoluteError(static_cast<double>(remath::sin<long double>(rwithinsin)), static_cast<double>(std::sin(rwithinsin)), 0.0000005);
            expectWithinAbsoluteError(static_cast<double>(remath::sin_fast<long double>(rwithinsin)), static_cast<double>(std::sin(rwithinsin)), 0.05);
            expectWithinAbsoluteError(static_cast<double>(remath::sin_faster<long double>(rwithinsin)), static_cast<double>(std::sin(rwithinsin)), 0.1);

            expectWithinAbsoluteError(static_cast<double>(remath::cos<long double>(rwithinsin)), static_cast<double>(std::cos(rwithinsin)), 0.0000005);
            expectWithinAbsoluteError(static_cast<double>(remath::cos_fast<long double>(rwithinsin)), static_cast<double>(std::cos(rwithinsin)), 0.005);
            expectWithinAbsoluteError(static_cast<double>(remath::cos_faster<long double>(rwithinsin)), static_cast<double>(std::cos(rwithinsin)), 0.1);
            expectWithinAbsoluteError(static_cast<double>(remath::cos_fastest<long double>(rwithinsin)), static_cast<double>(std::cos(rwithinsin)), 2.0); // lmao

            auto rwide = ((r * 2.0) - 1.0) * 100000.0;
            expectWithinAbsoluteError(static_cast<double>(remath::tanh<long double>(rwide)), static_cast<double>(std::tanh(rwide)), 0.0000005);
            expectWithinAbsoluteError(static_cast<double>(remath::tanh_fast<long double>(rwide)), static_cast<double>(std::tanh(rwide)), 0.005);
            expectWithinAbsoluteError(static_cast<double>(remath::tanh_faster<long double>(rwide)), static_cast<double>(std::tanh(rwide)), 0.05);

            rwide = abs(rwide);
            expectWithinAbsoluteError(static_cast<double>(remath::log2<long double>(rwide)), static_cast<double>(std::log2(rwide)), 0.0005);
            expectWithinAbsoluteError(static_cast<double>(remath::log2_fast<long double>(rwide)), static_cast<double>(std::log2(rwide)), 0.005);

            expectWithinAbsoluteError(static_cast<double>(remath::sqrt<double>(rwide)), static_cast<double>(std::sqrt(rwide)), 0.0005);
        }
    }

    DBG("f nearest below pi/2");
    DBG(juce::String::toDecimalStringWithSignificantFigures(std::nextafter(remath::halfpi<float>(), (float) 0.0), 70));
    DBG("d nearest below pi/2");
    DBG(juce::String::toDecimalStringWithSignificantFigures(std::nextafter(remath::halfpi<double>(), (double) 0.0), 70));
    DBG("l nearest below pi/2");
    DBG(juce::String::toDecimalStringWithSignificantFigures(std::nextafter(remath::halfpi<long double>(), (long double) 0.0), 70));
    DBG("\n");
    DBG("f nearest above -pi/2");
    DBG(juce::String::toDecimalStringWithSignificantFigures(std::nextafter(remath::neghalfpi<float>(), (float) 0.0), 70));
    DBG("d nearest above -pi/2");
    DBG(juce::String::toDecimalStringWithSignificantFigures(std::nextafter(remath::neghalfpi<double>(), (double) 0.0), 70));
    DBG("l nearest above -pi/2");
    DBG(juce::String::toDecimalStringWithSignificantFigures(std::nextafter(remath::neghalfpi<long double>(), (long double) 0.0), 70));
}