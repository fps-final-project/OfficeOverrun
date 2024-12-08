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

void RenderMaster::setLighting(const LightingData& data, const DirectX::XMVECTOR& flashlight_dir)
{
	const float cutoff = sqrt(3) / 2;

	DirectX::XMFLOAT3 flashlight_dir_f3;
	DirectX::XMStoreFloat3(&flashlight_dir_f3, flashlight_dir);

	m_modelRenderer->setLighting(data);
	m_animatedRenderer->setLighting(data);

	m_modelRenderer->setFlashlight(flashlight_dir_f3, cutoff);
	m_animatedRenderer->setFlashlight(flashlight_dir_f3, cutoff);
}

std::shared_ptr<AnimatedModelRenderer> RenderMaster::getAnimatedRenderer()
{
	this->SetRenderer(RendererType::ANIMATED);

	return m_animatedRenderer;
}

std::shared_ptr<ModelRenderer> RenderMaster::getModelRenderer()
{
	this->SetRenderer(RendererType::MODEL);

	return m_modelRenderer;
}

void RenderMaster::SetCullingMode(bool culling)
{
	if (culling)
	{
		m_modelRenderer->SetClockwiseCulling();
		m_animatedRenderer->SetClockwiseCulling();
	}
	else
	{
		m_modelRenderer->SetNoCulling();
		m_animatedRenderer->SetNoCulling();
	}
}

void RenderMaster::SetStencilBufferReferenceValue(UINT8 value)
{
	if(m_currentRenderer == RendererType::MODEL)
		m_modelRenderer->SetStencilBufferReferenceValue(value);
	if(m_currentRenderer == RendererType::ANIMATED)
		m_animatedRenderer->SetStencilBufferReferenceValue(value);
}

void RenderMaster::SetRenderer(RendererType type)
{
	if (m_currentRenderer == type) return;
	m_currentRenderer = type;
	
	if (type == RendererType::ANIMATED) m_animatedRenderer->use();
	if (type == RendererType::MODEL) m_modelRenderer->use();
}

UINT8 RenderMaster::GetCurrentStencilValue()
{
	UINT8 val1 = m_modelRenderer->GetStencilBufferValue();
	UINT8 val2 = m_animatedRenderer->GetStencilBufferValue();

	return std::max<UINT8>(m_modelRenderer->GetStencilBufferValue(), m_animatedRenderer->GetStencilBufferValue());
}

void RenderMaster::ClearStencilBuffer()
{
	if (m_currentRenderer == RendererType::MODEL)
		m_modelRenderer->ClearStencilBuffer();
	if (m_currentRenderer == RendererType::ANIMATED)
		m_animatedRenderer->ClearStencilBuffer();
}

void RenderMaster::CreateDeviceDependentResources()
{
	m_animatedRenderer->CreateDeviceDependentResources();
	m_modelRenderer->CreateDeviceDependentResources();
}
