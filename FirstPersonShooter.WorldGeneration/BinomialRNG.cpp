#include "pch.h"
#include "BinomialRNG.h"

using namespace WorldGenerator;

int WorldGenerator::BinomialRNG::RandIntInRange(int min, int max)
{
    std::mt19937& generator = RNGEngine::GetInstance()->generator;
    std::binomial_distribution<int> distribution(parameters.binomial_t, parameters.binomial_p);

    int result = distribution(generator);

    // Bound with min-max
    if (result < min)
        result = min;
    if (result > max)
        result = max;

    return result;
}
