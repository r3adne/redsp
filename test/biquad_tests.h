#ifndef REDSP_BIQUADTESTS_HEADERGUARD
#define REDSP_BIQUADTESTS_HEADERGUARD

#include <juce_core/juce_core.h>


using namespace juce;

struct BiquadTest : public UnitTest
{
    BiquadTest() : UnitTest("Biquad", "Filters") { }

private:

    void runTest() override;
};

#endif // REDSP_BIQUADTESTS_HEADERGUARD