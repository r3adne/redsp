//
// Created by ariadne lewis-towbes on 4/20/23.
//

#ifndef REDSP_SVFTESTS_HEADERGUARD
#define REDSP_SVFTESTS_HEADERGUARD


#include <juce_core/juce_core.h>
#include "../source/filters/svf.h"

#pragma once

using namespace juce;

struct SVFTest : public UnitTest
{
    SVFTest() : UnitTest("SVF", "Filters") { }

};

#endif // REDSP_SVFTESTS_HEADERGUARD