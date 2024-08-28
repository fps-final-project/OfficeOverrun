#pragma once
#include <memory>
#include <vector>
#include "AnimatedAssimpModel.h"
#include "Animator.h"


class Animable
{
public:
	Animable(std::shared_ptr<AnimatedAssimpModel> animatedModel);
	void setAnimation(std::string name);
	virtual void Draw();
	Animator m_animator;
	std::shared_ptr<AnimatedAssimpModel> m_animatedModel;
	DirectX::XMMATRIX m_model;
protected:
};