#include "pch.h"
#include "AnimatedModelRenderer.hpp"
#include <vector>

AnimatedModelRenderer::AnimatedModelRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: Base3DRenderer(deviceResources)
{
	DirectX::XMStoreFloat3(&m_PSConstantBufferData.light_pos, { 0.f, 1.f, 0.f, 0.f });
	this->CreateDeviceDependentResources();
}

AnimatedModelRenderer::~AnimatedModelRenderer()
{
	this->ReleaseDeviceDependentResources();
}

void AnimatedModelRenderer::ReleaseDeviceDependentResources()
{
	m_AnimationInverseTransformBuffer.Reset();
	m_AnimationTransformBuffer.Reset();
	Base3DRenderer::ReleaseDeviceDependentResources();
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

	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(AnimationConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			&m_AnimationTransformBuffer
		)
	);

	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			&m_AnimationInverseTransformBuffer
		)
	);

	this->CreateDeviceDependentResources_internal(L"SkeletalVertexShader.cso", L"SkeletalPixelShader.cso", vertexDesc);
}

void AnimatedModelRenderer::Render(const Animable& animable)
{
	if (!m_loadingComplete)
	{
		return;
	}

	DirectX::XMStoreFloat4x4(&m_VSConstantBufferData.model, DirectX::XMMatrixTranspose(animable.m_model));
	auto det = DirectX::XMMatrixDeterminant(animable.m_model);
	XMStoreFloat4x4(&m_VSConstantBufferData.inv_model, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, animable.m_model)));

	auto pose = animable.m_animator.getFinalTransformationMatricies(animable.m_animatedModel->m_transformData);

	for (int i = 0; i < pose.size(); i++)
	{
		DirectX::XMStoreFloat4x4(&m_AnimationTransformBufferData.pose[i], DirectX::XMMatrixTranspose(pose[i]));
		auto det = DirectX::XMMatrixDeterminant(pose[i]);
		XMStoreFloat4x4(&m_AnimationInverseTransformBufferData.pose[i], DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, pose[i])));
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	context->UpdateSubresource1(
		m_AnimationTransformBuffer.Get(),
		0,
		NULL,
		&m_AnimationTransformBufferData,
		0,
		0,
		0
	);

	context->UpdateSubresource1(
		m_AnimationInverseTransformBuffer.Get(),
		0,
		NULL,
		&m_AnimationInverseTransformBufferData,
		0,
		0,
		0
	);

	context->VSSetConstantBuffers1(
		2,
		1,
		m_AnimationTransformBuffer.GetAddressOf(),
		nullptr,
		nullptr
	);

	context->VSSetConstantBuffers1(
		3,
		1,
		m_AnimationInverseTransformBuffer.GetAddressOf(),
		nullptr,
		nullptr
	);

	Base3DRenderer::Render(*animable.m_animatedModel);
}
