#include "pch.h"
#include "UniformRNG.h"

int WorldGenerator::UniformRNG::RandIntInRange(int min, int max)
{
    std::mt19937& generator = RNGEngine::GetInstance()->generator;
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(generator);
}
