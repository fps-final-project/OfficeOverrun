#pragma once
#include <memory>
#include <vector>
#include "Drawable.hpp"
#include "Animator.h"
#include "RenderMaster.hpp"
#include "AnimatedAssimpModel.h"


class  __declspec(dllexport) Animable : public Drawable
{
public:
	Animable(std::shared_ptr<AnimatedModel> animatedModel);
	virtual ~Animable() {}
	// those 2 functions are code smells - think about how to call animator directly, is inheritance a good answer?
	// maybe its good idk
	inline void setModel(std::shared_ptr<AnimatedModel> animatedModel) { m_animatedModel = animatedModel; };
	void setAnimation(std::string name, float speed = 1.f, bool wrap = false);
	void setFallbackAnimation(std::string name);
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) = 0;
	bool isIdle() const { return m_animator.isIdle(); }
protected:
	Animator m_animator;
	std::shared_ptr<AnimatedModel> m_animatedModel;
};