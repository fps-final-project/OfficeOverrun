#include "pch.h"
#include "DistributionFactory.h"
#include "PoissonRNG.h"
#include "BinomialRNG.h"

using namespace WorldGenerator;

DistributionRNG* DistributionFactory::CreateRNG(Distribution distribution)
{
	switch (distribution)
	{
	case Uniform:
		return new UniformRNG();
	case Poisson:
		return new PoissonRNG();
	case Binomial:
		return new BinomialRNG();
	default:
		return nullptr;
	}
}
