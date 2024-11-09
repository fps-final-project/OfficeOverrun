#pragma once
#include <DirectXMath.h>
#include <memory>
#include "AnimatedAssimpModel.h"
#include "AnimatedObject.hpp"

class AnimatedObjectBuilder
{
public:
	AnimatedObjectBuilder& WithNewObject(std::shared_ptr<AnimatedModel> model);
	AnimatedObjectBuilder& WithPosition(DirectX::XMFLOAT3 pos);
	AnimatedObjectBuilder& WithRotation(DirectX::XMFLOAT3 rot);
	AnimatedObjectBuilder& WithVelocity(DirectX::XMFLOAT3 vel);
	AnimatedObjectBuilder& WithSize(DirectX::XMFLOAT3 size);
	AnimatedObjectBuilder& WithFallbackAnimation(std::string animationName);
	std::shared_ptr<AnimatedObject>& Build();
private:
	std::shared_ptr<AnimatedObject> m_object;
};