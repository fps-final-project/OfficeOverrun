#pragma once
#include "Hittable.hpp"
#include "Animable.hpp"

using namespace DirectX;

class  __declspec(dllexport) AnimatedEntity : public Hittable, public Animable
{
public:
	AnimatedEntity(std::shared_ptr<AnimatedModel> model, 
		XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, 0.f), 
		XMFLOAT3 size = DirectX::XMFLOAT3(1.f, 1.f, 1.f), 
		XMFLOAT3 rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f), 
		XMFLOAT3 velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	virtual ~AnimatedEntity() {}
	void Update(float dt);
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) override;

	void setPosition(DirectX::XMFLOAT3 pos);
	void setRotation(DirectX::XMFLOAT3 rot);
	void setVelocity(DirectX::XMFLOAT3 vel);

	inline XMFLOAT3 getPosition() { return position; }
	inline XMFLOAT3 getRotation() { return rotation; }
	inline XMFLOAT3 getVelocity() { return velocity; }

protected:
	XMFLOAT3 rotation;
	XMFLOAT3 velocity;
	XMFLOAT3 size;
};