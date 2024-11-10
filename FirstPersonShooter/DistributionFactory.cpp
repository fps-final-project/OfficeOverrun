#include "pch.h"
#include "DistributionFactory.h"

using namespace WorldGenerator;

DistributionRNG* DistributionFactory::CreateRNG(Distribution distribution)
{
	switch (distribution)
	{
	case Uniform:
		return new UniformRNG();
	default:
		return nullptr;
	}
}
