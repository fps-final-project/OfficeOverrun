#pragma once
#include <vector>
#include "RNG.h"
#include <algorithm>
#include <random> 

namespace WorldGenerator
{
	class RngUtils
	{
	public:
		static int RandIntInRange(int min, int max);
		static bool RandBool();
		template<typename T>
		static std::vector<T> ShuffleVector(std::vector<T> input);
	};
	template<typename T>
	inline std::vector<T> RngUtils::ShuffleVector(std::vector<T> input)
	{
		unsigned seed = RNG::GetInstance()->GetSeed();
		std::shuffle(input.begin(), input.end(), std::default_random_engine(seed));
		return input;
	}
}

