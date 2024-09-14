#pragma once
#include "Content/Base3DRenderer.h"
#include "Drawable.hpp"

class ModelRenderer : public Base3DRenderer<ModelViewProjectionConstantBuffer, LightingConstantBuffer, VertexData>
{
public:
	ModelRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	virtual ~ModelRenderer() override;
	virtual void CreateDeviceDependentResources() override;
	void Render(const Drawable& drawable);
};