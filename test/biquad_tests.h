#ifndef REDSP_BIQUADTESTS_HEADERGUARD
#define REDSP_BIQUADTESTS_HEADERGUARD

#include <juce_core/juce_core.h>
#include "../source/filters/biquad.h"

#pragma once

using namespace juce;

struct BiquadTest : public UnitTest
{
    BiquadTest() : UnitTest("Biquad", "Filters") { }

private:

    void runTest() override
    {
        auto b = std::make_unique<redsp::biquad<double, double, 1>>();
        {
            // process_does
            beginTest("process_double_nocrashes");
            for (int i = 0; i < 100; ++i)
            {
                b->calc_lp(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 1.0));
                b->process(getRandom().nextDouble());
            }
            for (int i = 0; i < 100; ++i)
            {
                b->calc_hp(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 1.0));
                b->process(getRandom().nextDouble());
            }
            for (int i = 0; i < 100; ++i)
            {
                b->calc_bp(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 1.0));
                b->process(getRandom().nextDouble());
            }
            for (int i = 0; i < 100; ++i)
            {
                b->calc_br(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 1.0));
                b->process(getRandom().nextDouble());
            }
            for (int i = 0; i < 100; ++i)
            {
                b->calc_ap(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 1.0));
                b->process(getRandom().nextDouble());
            }
        }

    }
};

static BiquadTest biquadTest;


#endif // REDSP_BIQUADTESTS_HEADERGUARD