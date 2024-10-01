#pragma once
#include <memory>
#include <vector>
#include "Drawable.hpp"
#include "Animator.h"
#include "RenderMaster.hpp"
#include "AnimatedAssimpModel.h"


class Animable : public Drawable
{
public:
	Animable(std::shared_ptr<AnimatedAssimpModel> animatedModel);
	virtual ~Animable() {}
	// those 2 functions are code smells - think about how to call animator directly, is inheritance a good answer?
	// maybe its good idk
	void setAnimation(std::string name, float speed = 1.f, bool wrap = false);
	void setFallbackAnimation(std::string name);
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;
	bool isIdle() { return m_animator.isIdle(); }
	friend class AnimatedModelRenderer;
protected:
	Animator m_animator;
	std::shared_ptr<AnimatedAssimpModel> m_animatedModel;
	DirectX::XMMATRIX m_model;
};