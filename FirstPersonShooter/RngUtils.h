#pragma once
#include <vector>
#include "RNGEngine.h"
#include <algorithm>
#include <random> 

namespace WorldGenerator
{
	class RngUtils
	{
	public:
		static int RandIntInRange(int min, int max); // includes <min,max> (inclusive)
		static bool RandBool();
		static bool RandBinWithProbabilty(float probability);
		template<typename T>
		static std::vector<T> ShuffleVector(std::vector<T> input);
		template<typename T>
		static T SelectRandomElement(std::vector<T> input);
	};

	inline int RngUtils::RandIntInRange(int min, int max)
	{
		return min + rand() % (max - min + 1);
	}

	template<typename T>
	inline std::vector<T> RngUtils::ShuffleVector(std::vector<T> input)
	{
		unsigned seed = RNGEngine::GetInstance()->GetSeed();
		std::shuffle(input.begin(), input.end(), std::default_random_engine(seed));
		return input;
	}
	template<typename T>
	inline T RngUtils::SelectRandomElement(std::vector<T> input)
	{
		return input[RandIntInRange(0, input.size() - 1)];
	}
}

