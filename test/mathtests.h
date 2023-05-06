//
// Created by ariadne lewis-towbes on 5/3/23.
//

#ifndef REDSP_TEST_H
#define REDSP_TEST_H

#include <juce_core/juce_core.h>

using namespace juce;

struct MathTest : public UnitTest
{
    MathTest() : UnitTest("MathTests", "General") { }

private:

    void runTest() override;

};


#endif //REDSP_TEST_H
