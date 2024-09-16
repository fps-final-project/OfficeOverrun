#include "pch.h"
#include "Entity.hpp"

Entity::Entity(std::shared_ptr<AssimpModel> model, XMFLOAT3 position, XMFLOAT3 size, XMFLOAT3 rotation, XMFLOAT3 velocity)
	: Drawable(model), Hittable(DirectX::BoundingBox(position, size)), 
	position(position), rotation(rotation), velocity(velocity),
	size(size)
{
}

void Entity::Update(float dt)
{
	this->position.x += this->velocity.x * dt;
	this->position.y += this->velocity.y * dt;
	this->position.z += this->velocity.z * dt;
	
	this->boundingBox.Center = this->position;

	DirectX::XMMATRIX rotationMatrix = 
		DirectX::XMMatrixRotationX(this->rotation.x) * 
		DirectX::XMMatrixRotationY(this->rotation.y) * 
		DirectX::XMMatrixRotationZ(this->rotation.z);
	
	XMVECTOR transformedExtents =
		DirectX::XMVector3Transform(
			DirectX::XMLoadFloat3(&this->size),
			rotationMatrix
		);

	this->boundingBox.Extents = XMFLOAT3(
		transformedExtents.m128_f32[0],
		transformedExtents.m128_f32[1],
		transformedExtents.m128_f32[2]
	);

	this->m_modelMatrix =
		DirectX::XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z)
		* DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z)
		* DirectX::XMMatrixScaling(0.2f, 0.2f, 0.2f);
}
 
