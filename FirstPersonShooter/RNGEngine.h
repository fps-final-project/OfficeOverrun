#pragma once
#include <time.h>

namespace WorldGenerator
{
	class RNGEngine
	{
	private:
		unsigned int seed;
	protected:
		RNGEngine() : seed(time(NULL))
		{
		}
		static RNGEngine* instance_;

	public:
		RNGEngine(RNGEngine& other) = delete;
		void operator=(const RNGEngine&) = delete;

		static RNGEngine* GetInstance();

		void SetSeed(unsigned int seed);

		unsigned int GetSeed();
	};
}