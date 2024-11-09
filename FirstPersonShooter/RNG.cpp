#include "pch.h"
#include "RNG.h"

using namespace WorldGenerator;

bool WorldGenerator::RNG::RandBool()
{
	return RandBoolWithProbabilty(0.5);
}

bool WorldGenerator::RNG::RandBoolWithProbabilty(float probability)
{
	std::bernoulli_distribution distribution(probability);
	return distribution(generator);
}
