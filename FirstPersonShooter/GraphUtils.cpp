#include "pch.h"
#include "GraphUtils.h"

int WorldGenerator::GraphUtils::FindValueIndexInMap(std::vector<int> map, int value)
{
    auto it = std::find(map.begin(), map.end(), value);
    return it - map.begin();
}
