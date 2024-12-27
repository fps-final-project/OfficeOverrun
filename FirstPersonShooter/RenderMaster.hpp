#pragma once
#include <memory>
#include "AnimatedModelRenderer.hpp"
#include "ModelRenderer.hpp"
#include "RendererType.hpp"


class __declspec(dllexport) RenderMaster
{
public:
	RenderMaster(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	~RenderMaster();
	void setupShaders(DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT3 cameraPos);
	void setLighting(DirectX::XMFLOAT3 muzzleFlashPos, bool muzzleOn, const DirectX::XMVECTOR& flashlight_dir);

	void renderSkybox(DirectX::XMFLOAT3 cameraPos);

	std::shared_ptr<AnimatedModelRenderer> getAnimatedRenderer();	
	std::shared_ptr<ModelRenderer> getModelRenderer();

	void SetCullingMode(bool culling);

	void SetRenderer(RendererType type);
	UINT8 GetCurrentStencilValue();
	void ClearStencilBuffer();
	void SetStencilBufferReferenceValue(UINT8 value);
	inline RendererType GetCurrentRenderType() { return m_currentRenderer; }

private:
	std::shared_ptr<AnimatedModelRenderer> m_animatedRenderer;
	std::shared_ptr<ModelRenderer> m_modelRenderer;

	RendererType m_currentRenderer;
};