#pragma once
#include "../FirstPersonShooter.WorldGeneration/DistributionParameters.h"

namespace WorldGenerator
{
	class DistributionRNG
	{
	protected:
		DistributionParameters parameters;
	public:
		virtual int RandIntInRange(int min, int max) = 0;
		void SetParameters(const DistributionParameters &params);
		DistributionRNG();
	};
}