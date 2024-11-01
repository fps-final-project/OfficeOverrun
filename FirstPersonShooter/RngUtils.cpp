#include "pch.h"
#include "RngUtils.h"
#include "pch.h"
#include <algorithm>

using namespace WorldGenerator;

bool RngUtils::RandBool()
{
    return rand() % 2 == 1;
}

bool WorldGenerator::RngUtils::RandBinWithProbabilty(float probability)
{
    return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX + 1)) < probability;
}
