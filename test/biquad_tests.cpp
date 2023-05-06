
#include <juce_core/juce_core.h>
#include "../source/filters/biquad.h"
#include "biquad_tests.h"

using namespace juce;

void BiquadTest::runTest()
{
    auto b = std::make_unique<redsp::biquad<double, double, 1>>();
    {
        // process_does
        beginTest("process_double_nocrashes");
        for (int i = 0; i < 100; ++i)
        {
            b->calc_lp(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 1.0));
            for (int j = 0; j < 100000; ++j)
            {
                b->process(getRandom().nextDouble());
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            b->calc_hp(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 1.0));
            for (int j = 0; j < 100000; ++j)
            {
                b->process(getRandom().nextDouble());
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            b->calc_bp(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 1.0));
            for (int j = 0; j < 100000; ++j)
            {
                b->process(getRandom().nextDouble());
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            b->calc_br(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 1.0));
            for (int j = 0; j < 100000; ++j)
            {
                b->process(getRandom().nextDouble());
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            b->calc_ap(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 1.0));
            for (int j = 0; j < 100000; ++j)
            {
                b->process(getRandom().nextDouble());
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            b->calc_peak(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 16.0), fmod(getRandom().nextDouble(), 48.0) - 24.0);
            for (int j = 0; j < 100000; ++j)
            {
                b->process(getRandom().nextDouble());
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            b->calc_hs(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 16.0), fmod(getRandom().nextDouble(), 48.0) - 24.0);
            for (int j = 0; j < 100000; ++j)
            {
                b->process(getRandom().nextDouble());
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            b->calc_ls(fmod(getRandom().nextDouble(), 1.0), fmod(getRandom().nextDouble(), 16.0), fmod(getRandom().nextDouble(), 48.0) - 24.0);
            for (int j = 0; j < 100000; ++j)
            {
                b->process(getRandom().nextDouble());
            }
        }
    }
}
