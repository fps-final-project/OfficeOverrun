#include "pch.h"
#include "ObjectBuilder.hpp"

ObjectBuilder& ObjectBuilder::WithNewObject(std::shared_ptr<AssimpModel> model)
{
	m_object.reset(new Object(model));
	return *this;
}

ObjectBuilder& ObjectBuilder::WithPosition(DirectX::XMFLOAT3 pos)
{
	m_object->position = pos;
	return *this;
}

ObjectBuilder& ObjectBuilder::WithRotation(DirectX::XMFLOAT3 rot)
{
	m_object->rotation = rot;
	return *this;

}

ObjectBuilder& ObjectBuilder::WithVelocity(DirectX::XMFLOAT3 vel)
{
	m_object->velocity = vel;
	return *this;
}

ObjectBuilder& ObjectBuilder::WithSize(DirectX::XMFLOAT3 size)
{
	m_object->size = size;
	return *this;
}

ObjectBuilder& ObjectBuilder::WithPickable(bool pickable)
{
	m_object->pickable = pickable;
	return *this;
}

Object* ObjectBuilder::Build()
{
	return m_object.get();
}
