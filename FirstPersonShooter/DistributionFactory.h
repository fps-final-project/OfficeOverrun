#include "DistributionRNG.h"
#include "Distribution.h"
#include "UniformRNG.h"

#pragma once

namespace WorldGenerator
{
	class DistributionFactory
	{
	public:
		static DistributionRNG* CreateRNG(Distribution distribution);
	};
}

