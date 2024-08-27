#pragma once
#include <memory>
#include <vector>
#include "AnimatedAssimpModel.h"
#include "Animator.h"


class Animable
{
public:
	virtual void Draw() {}
	Animator m_animator;
	std::shared_ptr<AnimatedAssimpModel> m_animatedModel;
protected:
};