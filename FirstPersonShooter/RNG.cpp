#include "pch.h"
#include "RNG.h"

using namespace WorldGenerator;

bool RNG::RandBool()
{
	return RandBoolWithProbabilty(0.5);
}

bool RNG::RandBoolWithProbabilty(double probability)
{
	std::mt19937 &generator = RNGEngine::GetInstance()->generator;
	std::bernoulli_distribution distribution(probability);

	return distribution(generator);
}

float WorldGenerator::RNG::RandFloatInRange(float min, float max)
{
	if (min == max)
		return min;

	std::mt19937& generator = RNGEngine::GetInstance()->generator;
	std::uniform_real_distribution distribution(min, max);

	return distribution(generator);
}

int RNG::RandIntInRange(int min, int max, Distribution distribution)
{
	auto rng = DistributionFactory::CreateRNG(distribution);
	return rng->RandIntInRange(min, max);
}

int WorldGenerator::RNG::RandIntInRange(int min, int max, Distribution distribution, const DistributionParameters& parameters)
{
	auto rng = DistributionFactory::CreateRNG(distribution);
	rng->SetParameters(parameters);
	return rng->RandIntInRange(min, max);
}
