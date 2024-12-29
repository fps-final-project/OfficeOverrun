#include "pch.h"
#include "DistributionRNG.h"

using namespace WorldGenerator;

void WorldGenerator::DistributionRNG::SetParameters(const DistributionParameters& params)
{
	parameters = params;
}

WorldGenerator::DistributionRNG::DistributionRNG()
{
	DistributionParameters parameters;
}
