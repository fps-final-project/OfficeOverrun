#pragma once
#include "Content\Base3DRenderer.h"

class AnimatedModel;

class __declspec(dllexport) AnimatedModelRenderer : public Base3DRenderer<ModelViewProjectionConstantBuffer, AnimatedVertexData>
{
public:
	AnimatedModelRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	virtual ~AnimatedModelRenderer() override;
	virtual void ReleaseDeviceDependentResources() override;
	virtual void CreateDeviceDependentResources() override;
	void Render(const AnimatedModel& animatedModel,
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotation,
		const std::vector<DirectX::XMMATRIX>& finalBonesMatricies);
private:
	static const int nBuffers = 2;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_AnimationTransformBuffer[nBuffers];
	AnimationConstantBuffer m_AnimationTransformBufferData[nBuffers];
};