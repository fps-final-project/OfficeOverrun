#include "pch.h"
#include "AnimatedEntity.hpp"

AnimatedEntity::AnimatedEntity(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 velocity)
	: position(position), rotation(rotation), velocity(velocity)
{

}

AnimatedModelDrawRequest AnimatedEntity::Update(float dt)
{
	AnimatedModelDrawRequest request;

	this->m_animator.updateAnimation(this->m_animatedModel->m_rootJoint, this->m_animatedModel->m_BoneInfoMap, dt);

	request.m_animatedModel = this->m_animatedModel;
	request.m_animator = this->m_animator;

	request.model = DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
	return request;
}
