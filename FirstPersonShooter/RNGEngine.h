#pragma once
#include <time.h>
#include <random>

namespace WorldGenerator
{
	class RNGEngine
	{
	private:
		std::mt19937 generator;
	protected:
		RNGEngine() : generator(std::mt19937())
		{
		}
		static RNGEngine* instance_;

	public:
		RNGEngine(RNGEngine& other) = delete;
		void operator=(const RNGEngine&) = delete;

		static RNGEngine* GetInstance();

		void SetSeed(unsigned int seed);

		std::mt19937 GetGenerator();
	};
}