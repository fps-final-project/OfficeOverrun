#include <vector>
#include <random>

#include "DistributionRNG.h"

#pragma once

namespace WorldGenerator
{
	class UniformRNG : public DistributionRNG
	{
	public:
		int RandIntInRange(int min, int max) override;
	};

}

