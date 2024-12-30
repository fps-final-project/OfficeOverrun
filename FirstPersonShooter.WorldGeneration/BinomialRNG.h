#include <vector>
#include <random>
#include "DistributionRNG.h"
#include "RNGEngine.h"

#pragma once

namespace WorldGenerator
{
	class BinomialRNG : public DistributionRNG
	{
	public:
		int RandIntInRange(int min, int max) override;
	};

}