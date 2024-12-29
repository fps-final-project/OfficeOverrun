#include "pch.h"
#include "DistributionFactory.h"
#include "PoissonRNG.h"

using namespace WorldGenerator;

DistributionRNG* DistributionFactory::CreateRNG(Distribution distribution)
{
	switch (distribution)
	{
	case Uniform:
		return new UniformRNG();
	case Poisson:
		return new PoissonRNG();
	default:
		return nullptr;
	}
}
