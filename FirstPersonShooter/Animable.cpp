#include "pch.h"
#include "Animable.hpp"

Animable::Animable(std::shared_ptr<AnimatedAssimpModel> animatedModel)
	: m_animatedModel(animatedModel), m_model(DirectX::XMMatrixIdentity()), m_animator(Animator())
{

}

void Animable::setAnimation(std::string name, float speed, bool wrap)
{
	if (this->m_animatedModel->m_animations.find(name) != this->m_animatedModel->m_animations.end())
	{
		m_animator.playAnimation(this->m_animatedModel->m_animations[name], speed, wrap);
	}
}

void Animable::setFallbackAnimation(std::string name)
{
	if (this->m_animatedModel->m_animations.find(name) != this->m_animatedModel->m_animations.end())
	{
		m_animator.setFallbackAnimation(this->m_animatedModel->m_animations[name]);
	}
}

void Animable::Render(std::shared_ptr<RenderMaster> renderMaster)
{
	auto renderer = renderMaster->getAnimatedRenderer();
	renderer->Render(*this);
}
