#include "pch.h"
#include "RenderMaster.hpp"

RenderMaster::RenderMaster(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	m_modelRenderer = std::unique_ptr<ModelRenderer>(new ModelRenderer(deviceResources));
	m_animatedRenderer = std::unique_ptr<AnimatedModelRenderer>(new AnimatedModelRenderer(deviceResources));
}

RenderMaster::~RenderMaster()
{
	m_modelRenderer->ReleaseDeviceDependentResources();
	m_animatedRenderer->ReleaseDeviceDependentResources();
}

void RenderMaster::setupShaders(DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT3 cameraPos)
{
	m_modelRenderer->setProjectionMatrix(projectionMatrix);
	m_modelRenderer->setViewMatrix(viewMatrix);
	m_modelRenderer->setCameraPosition(cameraPos);

	m_animatedRenderer->setProjectionMatrix(projectionMatrix);
	m_animatedRenderer->setViewMatrix(viewMatrix);
	m_animatedRenderer->setCameraPosition(cameraPos);

	m_animatedRenderer->use();
	m_currentRenderer = RendererType::ANIMATED;
}

void RenderMaster::setLighting(const LightingData& data)
{
	m_modelRenderer->setLighting(data);
	m_animatedRenderer->setLighting(data);
}

std::shared_ptr<AnimatedModelRenderer> RenderMaster::getAnimatedRenderer()
{
	if (m_currentRenderer != RendererType::ANIMATED)
	{
		m_animatedRenderer->use();
		m_currentRenderer = RendererType::ANIMATED;
	}

	return m_animatedRenderer;
}

std::shared_ptr<ModelRenderer> RenderMaster::getModelRenderer()
{
	if (m_currentRenderer != RendererType::MODEL)
	{
		m_modelRenderer->use();
		m_currentRenderer = RendererType::MODEL;
	}

	return m_modelRenderer;
}
