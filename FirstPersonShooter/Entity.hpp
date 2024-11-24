#pragma once

#include "Drawable.hpp"
#include "Hittable.hpp"

using namespace DirectX;

class __declspec(dllexport) Entity : public Drawable, public Hittable
{
public:
	Entity(std::shared_ptr<Model> model,
		XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, 0.f),
		XMFLOAT3 size = DirectX::XMFLOAT3(1.f, 1.f, 1.f),
		XMFLOAT3 rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f),
		XMFLOAT3 velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	virtual ~Entity() {}
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;
	virtual void Update(float dt);

	inline XMFLOAT3 GetPosition() const { return position; }
	inline XMFLOAT3 GetRotation() const { return rotation; }
	inline XMFLOAT3 GetSize() const { return size; }
protected:
	std::shared_ptr<Model> m_model;
	DirectX::XMMATRIX m_modelMatrix;

	XMFLOAT3 rotation;
	XMFLOAT3 size;
	XMFLOAT3 velocity;
};

