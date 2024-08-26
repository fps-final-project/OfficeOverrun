#pragma once
#include <memory>
#include "AnimatedAssimpModel.h"


class Animable
{
public:
	virtual void Draw() = 0;
protected:
	std::shared_ptr<AnimatedAssimpModel> m_model;
};