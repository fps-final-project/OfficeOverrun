#include <vector>
#include <random>
#include "DistributionRNG.h"
#include "RNGEngine.h"

#pragma once

namespace WorldGenerator
{
	class PoissonRNG : public DistributionRNG
	{
	public:
		int RandIntInRange(int min, int max) override;
	};

}

