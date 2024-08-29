#pragma once
#include "Content\Base3DRenderer.h"
#include "Animable.hpp"

class AnimatedModelRenderer : public Base3DRenderer<AnimationConstantBuffer, LightingConstantBuffer, AnimatedVertexData>
{
public:
	AnimatedModelRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	virtual void CreateDeviceDependentResources() override;
	void Render(const Animable& animable);
};