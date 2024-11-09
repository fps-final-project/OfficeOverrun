#include "pch.h"
#include "UniformRNG.h"

int WorldGenerator::UniformRNG::RandIntInRange(int min, int max)
{
    std::uniform_int_distribution<int> distribution(1, 6);
    return distribution(generator);
}
