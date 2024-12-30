#include "pch.h"
#include "GeometryUtils.h"
#include <stdlib.h>

using namespace WorldGenerator;

bool GeometryUtils::LinesIntersect1D(int s1, int e1, int s2, int e2)
{
    return (s1 <= s2 && e1 >= s2) || (s2 <= s1 && e2 >= s1);
}

bool GeometryUtils::LinesIntersectByEnds1D(int s1, int e1, int s2, int e2)
{
    return e1 == s2 || e2 == s1;
}


std::tuple<int,int> GeometryUtils::ComputeLinesIntersection1D(int s1, int e1, int s2, int e2)
{
    int l = max(s1, s2);
    int r = min(e1, e2);
    int len = r - l;
    return std::tuple<int, int>(l, len < 0 ? 0 : len);
}

bool WorldGenerator::GeometryUtils::BoxesIntersect(DirectX::XMFLOAT2 pos1, DirectX::XMFLOAT2 size1, DirectX::XMFLOAT2 pos2, DirectX::XMFLOAT2 size2)
{
    return pos1.x <= pos2.x + size2.x
        && pos1.x + size1.x >= pos2.x
        && pos1.y + size1.y >= pos2.y
        && pos1.y <= pos2.y + size2.y;
}
