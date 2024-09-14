#pragma once
#include <memory>
#include <vector>
#include "AnimatedAssimpModel.h"
#include "Animator.h"


class Animable
{
public:
	Animable(std::shared_ptr<AnimatedAssimpModel> animatedModel);
	// those 2 functions are code smells - think about how to call animator directly, is inheritance a good answer?
	void setAnimation(std::string name, float speed = 1.f, bool wrap = false);
	void setFallbackAnimation(std::string name);
	//virtual void Draw();
	bool isIdle() { return m_animator.isIdle(); }
	friend class AnimatedModelRenderer;
protected:
	Animator m_animator;
	std::shared_ptr<AnimatedAssimpModel> m_animatedModel;
	DirectX::XMMATRIX m_model;
};