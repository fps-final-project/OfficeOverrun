#include "pch.h"
#include "AnimatedEntity.hpp"

AnimatedEntity::AnimatedEntity(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 velocity)
	: position(position), rotation(rotation), velocity(velocity)
{

}

void AnimatedEntity::Update(float dt)
{
	this->m_animator.updateAnimation(this->m_animatedModel->m_rootJoint, this->m_animatedModel->m_BoneInfoMap, dt);
	this->m_model = DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
}
