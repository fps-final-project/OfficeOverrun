#include "pch.h"
#include "AnimatedEntity.hpp"

AnimatedEntity::AnimatedEntity(std::shared_ptr<AnimatedModel> model, XMFLOAT3 position, XMFLOAT3 size, XMFLOAT3 rotation, XMFLOAT3 velocity)
	: Animable(model), Hittable(DirectX::BoundingBox(position, size), position), rotation(rotation), velocity(velocity), size(size)
{

}

void AnimatedEntity::Update(float dt)
{
	this->m_animator.updateAnimation(this->m_animatedModel->m_rootJoint, this->m_animatedModel->m_BoneInfoMap, dt);
}

void AnimatedEntity::Render(std::shared_ptr<RenderMaster> renderMaster)
{
	auto renderer = renderMaster->getAnimatedRenderer();
	renderer->Render(*m_animatedModel, position, size, rotation, m_animator.m_finalBoneMatrices);
}

void AnimatedEntity::setPosition(DirectX::XMFLOAT3 pos)
{
	this->position = pos;
}

void AnimatedEntity::setRotation(DirectX::XMFLOAT3 rot)
{
	this->rotation = rot;
}

void AnimatedEntity::setVelocity(DirectX::XMFLOAT3 vel)
{
	this->velocity = vel;
}
