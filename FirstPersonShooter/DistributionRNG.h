#pragma once

namespace WorldGenerator
{
	class DistributionRNG
	{
	public:
		virtual int RandIntInRange(int min, int max) = 0;
	};
}