#include "pch.h"
#include "Entity.hpp"

Entity::Entity(std::shared_ptr<AssimpModel> model, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 velocity)
	: Drawable(model), Hittable(DirectX::BoundingBox(position, XMFLOAT3(5.0f, 2.0f, 2.0f))), position(position), rotation(rotation), velocity(velocity)
{
}

void Entity::Update(float dt)
{
	this->position.x += this->velocity.x * dt;
	this->position.y += this->velocity.y * dt;
	this->position.z += this->velocity.z * dt;
	this->boundingBox.Center = this->position;

	this->m_modelMatrix =
		DirectX::XMMatrixRotationY(this->rotation.y)
		* DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z)
		* DirectX::XMMatrixScaling(0.2f, 0.2f, 0.2f);
}
 
