#pragma once
#include "Content\Base3DRenderer.h"

class Animable;

class AnimatedModelRenderer : public Base3DRenderer<ModelViewProjectionConstantBuffer, AnimatedVertexData>
{
public:
	AnimatedModelRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	virtual ~AnimatedModelRenderer() override;
	virtual void ReleaseDeviceDependentResources() override;
	virtual void CreateDeviceDependentResources() override;
	void Render(const Animable& animable);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_AnimationTransformBuffer, m_AnimationInverseTransformBuffer;
	AnimationConstantBuffer m_AnimationTransformBufferData, m_AnimationInverseTransformBufferData;
};