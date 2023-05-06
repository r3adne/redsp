//
// Created by ariadne lewis-towbes on 5/3/23.
//

#ifndef REDSP_TEST_H
#define REDSP_TEST_H

#include <juce_core/juce_core.h>
//#include "" include demo objects

using namespace juce;

struct DemoTest : public UnitTest
{
    DemoTest() : UnitTest("Demo", "Category") { }

private:

    void runTest() override;
};


#endif //REDSP_TEST_H
