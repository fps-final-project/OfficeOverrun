#include <vector>
#include <random>
#include "RNGEngine.h"
#include "DistributionFactory.h"

#pragma once

namespace WorldGenerator
{
	class RNG
	{
	public:
		static bool RandBool();
		static bool RandBoolWithProbabilty(double probability);
		static int RandIntInRange(int min, int max, Distribution distribution = Uniform);
		template<typename T>
		static T SelectRandomElement(std::vector<T> input, Distribution distribution = Uniform);
	};

	template<typename T>
	inline T RNG::SelectRandomElement(std::vector<T> input, Distribution distribution)
	{
		return input[RandIntInRange(0, input.size() - 1, distribution)];
	}
}