#include "pch.h"
#include "PoissonRNG.h"

using namespace WorldGenerator;

int PoissonRNG::RandIntInRange(int min, int max)
{
    std::mt19937& generator = RNGEngine::GetInstance()->generator;
    std::poisson_distribution<int> distribution(parameters.poisson_lambda);

    int result = distribution(generator);

    // Bound with min-max
    if (result < min)
        result = min;
    if (result > max)
        result = max;

    return result;
}
