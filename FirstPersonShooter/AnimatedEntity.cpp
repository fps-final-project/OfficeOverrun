#include "pch.h"
#include "AnimatedEntity.hpp"

AnimatedEntity::AnimatedEntity(std::shared_ptr<AnimatedAssimpModel> model, XMFLOAT3 position, XMFLOAT3 size, XMFLOAT3 rotation, XMFLOAT3 velocity)
	: Animable(model), Hittable(DirectX::BoundingBox()), position(position), rotation(rotation), velocity(velocity), size(size)
{

}

void AnimatedEntity::Update(float dt)
{
	this->m_animator.updateAnimation(this->m_animatedModel->m_rootJoint, this->m_animatedModel->m_BoneInfoMap, dt);
	/*this->m_model = DirectX::XMMatrixRotationZ(this->rotation.z)
		* DirectX::XMMatrixRotationX(this->rotation.x)
		* DirectX::XMMatrixRotationY(this->rotation.y)
		* DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);*/
	this->m_model = DirectX::XMMatrixScaling(this->size.x, this->size.y, this->size.z)
		* DirectX::XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z)
		* DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);

}

void AnimatedEntity::setPosition(DirectX::XMFLOAT3 pos)
{
	this->position = pos;
}

void AnimatedEntity::setRotation(DirectX::XMFLOAT3 rot)
{
	this->rotation = rot;
}
