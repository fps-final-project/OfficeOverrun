#pragma once
#include "Content/Base3DRenderer.h"

class Entity;

class __declspec(dllexport) ModelRenderer : public Base3DRenderer<ModelViewProjectionConstantBuffer, VertexData>
{
public:
	ModelRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	virtual ~ModelRenderer() override;
	virtual void CreateDeviceDependentResources() override;
	void Render(const Model& model, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotation);
};