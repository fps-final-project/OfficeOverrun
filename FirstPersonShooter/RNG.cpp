#include "pch.h"
#include "RNG.h"
#include <stdlib.h>

RNG* RNG::instance_;

RNG* RNG::GetInstance()
{
	if (instance_ == nullptr)
		instance_ = new RNG();
	else
		return instance_;
}

void RNG::SetSeed(unsigned int seed)
{
	this->seed = seed;
	srand(seed);
}

unsigned int RNG::GetSeed()
{
	return this->seed;
}
