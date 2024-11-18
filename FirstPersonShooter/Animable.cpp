#include "pch.h"
#include "Animable.hpp"

Animable::Animable(std::shared_ptr<AnimatedModel> animatedModel)
	: m_animatedModel(animatedModel), m_animator(Animator())
{

}

void Animable::setAnimation(std::string name, float speed, bool wrap)
{
	if (this->m_animatedModel->m_animations.find(name) != this->m_animatedModel->m_animations.end())
	{
		m_animator.playAnimation(this->m_animatedModel->m_animations[name], speed, wrap);
	}
	else m_animator.playAnimation(nullptr);
}

void Animable::setFallbackAnimation(std::string name)
{
	if (this->m_animatedModel->m_animations.find(name) != this->m_animatedModel->m_animations.end())
	{
		m_animator.setFallbackAnimation(this->m_animatedModel->m_animations[name]);
	}
}