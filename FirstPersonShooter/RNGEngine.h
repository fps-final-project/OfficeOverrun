#pragma once
#include <time.h>
#include <random>

namespace WorldGenerator
{
	class RNGEngine
	{
	protected:
		RNGEngine() : generator(std::mt19937())
		{
		}
		static RNGEngine* instance_;
	public:
		std::mt19937 generator;
		RNGEngine(RNGEngine& other) = delete;
		void operator=(const RNGEngine&) = delete;

		static RNGEngine* GetInstance();

		void SetSeed(unsigned int seed);
	};
}