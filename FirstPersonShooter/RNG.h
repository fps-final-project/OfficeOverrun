#include <vector>
#include <random>
#include "RNGEngine.h"

#pragma once

namespace WorldGenerator
{
	class RNG
	{
	protected:
		std::mt19937 generator = RNGEngine::GetInstance()->GetGenerator();
	public:
		virtual int RandIntInRange(int min, int max) = 0; // includes <min,max> (inclusive)
		bool RandBool();
		bool RandBoolWithProbabilty(float probability);
	};
}