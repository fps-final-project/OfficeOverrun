#include "pch.h"
#include "Utils.h"
#include <stdlib.h>

using namespace WorldGenerator;

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
    if (s1 <= s2 && e1 >= s2)
        return std::tuple<int, int>(s2, e1 - s2);
    else
        return std::tuple<int, int>(s1, e2 - s1);
}