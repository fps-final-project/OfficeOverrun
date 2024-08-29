#include "pch.h"
#include "Entity.hpp"

Entity::Entity(std::shared_ptr<AssimpModel> model, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 velocity)
	: Drawable(model), Hittable(DirectX::BoundingBox()), position(position), rotation(rotation), velocity(velocity)
{
}

void Entity::Update(float dt)
{
	const float period = 4.f;
	//this->rotation.y = fmod(this->rotation.y + DirectX::XM_2PI * dt / period, DirectX::XM_2PI);
	this->rotation.y = DirectX::XM_PI / 2;

	this->m_modelMatrix =
		DirectX::XMMatrixRotationY(this->rotation.y)
		* DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z)
		* DirectX::XMMatrixScaling(0.2f, 0.2f, 0.2f);
}
 
