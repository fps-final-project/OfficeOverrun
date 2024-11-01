#include "pch.h"
#include "Utils.h"
#include <stdlib.h>

using namespace WorldGenerator;

int Utils::RandIntInRange(int min, int max)
{
    return min + rand() % (max - min + 1);
}

bool Utils::RandBool()
{
    return rand() % 2 == 1;
}

bool Utils::LinesIntersect1D(int s1, int e1, int s2, int e2)
{
    return (s1 <= s2 && e1 >= s2) || (s2 <= s1 && e2 >= s1);
}

bool Utils::LinesIntersectByEnds1D(int s1, int e1, int s2, int e2)
{
    return e1 == s2 || e2 == s1;
}


std::tuple<int,int> Utils::ComputeLinesIntersection1D(int s1, int e1, int s2, int e2)
{
    int l = max(s1, s2);
    int r = min(e1, e2);
    int len = r - l;
    return std::tuple<int, int>(l, len < 0 ? 0 : len);
}