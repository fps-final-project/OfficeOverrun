#pragma once

#include "Drawable.hpp"
#include "Hittable.hpp"

using namespace DirectX;

class Entity : public Drawable, public Hittable
{
public:
	Entity(std::shared_ptr<AssimpModel> model,
		XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, 0.f),
		XMFLOAT3 size = DirectX::XMFLOAT3(1.f, 1.f, 1.f),
		XMFLOAT3 rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f),
		XMFLOAT3 velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	virtual ~Entity() {}
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;
	virtual void Update(float dt);
	friend class ModelRenderer;
protected:
	std::shared_ptr<AssimpModel> m_model;
	DirectX::XMMATRIX m_modelMatrix;

	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 size;
	XMFLOAT3 velocity;
};

