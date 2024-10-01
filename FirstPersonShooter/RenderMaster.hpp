#pragma once
#include <memory>
#include "AnimatedModelRenderer.hpp"
#include "ModelRenderer.hpp"
#include "RendererType.hpp"


class RenderMaster
{
public:
	RenderMaster(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	~RenderMaster();
	void setupShaders(DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT3 cameraPos);

	std::shared_ptr<AnimatedModelRenderer> getAnimatedRenderer();	
	std::shared_ptr<ModelRenderer> getModelRenderer();

private:
	std::shared_ptr<AnimatedModelRenderer> m_animatedRenderer;
	std::shared_ptr<ModelRenderer> m_modelRenderer;

	RendererType m_currentRenderer;
};