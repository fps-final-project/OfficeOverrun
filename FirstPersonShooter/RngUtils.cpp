#include "pch.h"
#include "RngUtils.h"
#include "pch.h"
#include <algorithm>

using namespace WorldGenerator;

int RngUtils::RandIntInRange(int min, int max)
{
    return min + rand() % (max - min + 1);
}

bool RngUtils::RandBool()
{
    return rand() % 2 == 1;
}
