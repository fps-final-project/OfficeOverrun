#include "pch.h"
#include "AnimatedModelRenderer.hpp"
#include <vector>

AnimatedModelRenderer::AnimatedModelRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: Base3DRenderer(deviceResources)
{
	this->CreateDeviceDependentResources();
}

void AnimatedModelRenderer::CreateDeviceDependentResources()
{
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDesc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 3 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 5 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "FINAL_ID", 0, DXGI_FORMAT_R32_SINT, 0, 8 * sizeof(int), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	this->CreateDeviceDependentResources_internal(L"SkeletalVertexShader.cso", L"SkeletalPixelShader.cso", vertexDesc);
}

void AnimatedModelRenderer::Render(const Animable& animable)
{
	if (!m_loadingComplete)
	{
		return;
	}

	auto modelMatrix = DirectX::XMMatrixScaling(1.f, 1.f, 1.f);
	DirectX::XMStoreFloat4x4(&m_VSConstantBufferData.model, DirectX::XMMatrixTranspose(animable.m_model));
	auto pose = animable.m_animator.getFinalTransformationMatricies(animable.m_animatedModel->m_transformData);
	for (int i = 0; i < pose.size(); i++)
	{
		DirectX::XMStoreFloat4x4(&m_VSConstantBufferData.pose[i], DirectX::XMMatrixTranspose(pose[i]));
	}

	Base3DRenderer::Render(*animable.m_animatedModel);
}
