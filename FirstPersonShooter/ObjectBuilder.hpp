#pragma once
#include "Object.hpp"

class ObjectBuilder
{
public:
	ObjectBuilder& WithNewObject(std::shared_ptr<Model> model);
	ObjectBuilder& WithPosition(DirectX::XMFLOAT3 pos);
	ObjectBuilder& WithRotation(DirectX::XMFLOAT3 rot);
	ObjectBuilder& WithVelocity(DirectX::XMFLOAT3 vel);
	ObjectBuilder& WithSize(DirectX::XMFLOAT3 size);
	ObjectBuilder& WithPickable(bool pickable);
	std::shared_ptr<Object>& Build();
private:
	std::shared_ptr<Object> m_object;
};

