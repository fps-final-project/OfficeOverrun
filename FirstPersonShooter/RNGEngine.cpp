#include "pch.h"
#include "RNGEngine.h"
#include <stdlib.h>

using namespace WorldGenerator;

RNGEngine* RNGEngine::instance_;

RNGEngine* RNGEngine::GetInstance()
{
	if (instance_ == nullptr)
		instance_ = new RNGEngine();
	return instance_;
}

void RNGEngine::SetSeed(unsigned int seed)
{
	this->generator.seed(seed);
}
