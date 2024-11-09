#include "RNG.h"

#pragma once

namespace WorldGenerator
{
	class UniformRNG : public RNG
	{
		// Inherited via RNG
		int RandIntInRange(int min, int max) override;
	};
}

