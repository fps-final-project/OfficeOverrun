#pragma once
#include <time.h>

namespace WorldGenerator
{
	class RNG
	{
	private:
		unsigned int seed;
	protected:
		RNG() : seed(time(NULL))
		{
		}
		static RNG* instance_;

	public:
		RNG(RNG& other) = delete;
		void operator=(const RNG&) = delete;

		static RNG* GetInstance();

		void SetSeed(unsigned int seed);

		unsigned int GetSeed();
	};
}