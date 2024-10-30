#include "pch.h"
#include "RngUtils.h"
#include "pch.h"
#include <algorithm>

using namespace WorldGenerator;

bool RngUtils::RandBool()
{
    return rand() % 2 == 1;
}
