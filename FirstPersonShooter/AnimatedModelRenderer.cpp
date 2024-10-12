#include "pch.h"
#include "AnimatedModelRenderer.hpp"
#include "Animable.hpp"
#include <vector>

//const int AnimatedModelRenderer::nBuffers = 3;

AnimatedModelRenderer::AnimatedModelRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: Base3DRenderer(deviceResources)
{
	this->CreateDeviceDependentResources();
}

AnimatedModelRenderer::~AnimatedModelRenderer()
{
	this->ReleaseDeviceDependentResources();
}

void AnimatedModelRenderer::ReleaseDeviceDependentResources()
{
	for (int i = 0; i < nBuffers; i++)
	{
		m_AnimationInverseTransformBuffer[i].Reset();
		m_AnimationTransformBuffer[i].Reset();
	}
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

	for (int i = 0; i < nBuffers; i++)
	{
		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(AnimationConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_AnimationTransformBuffer[i]
			)
		);

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_AnimationInverseTransformBuffer[i]
			)
		);
	}

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
		int bufferIdx = i / 50;
		int poseIdx = i - bufferIdx * AnimationConstantBuffer::MAX_BONES;

		DirectX::XMStoreFloat4x4(&m_AnimationTransformBufferData[bufferIdx].pose[poseIdx], DirectX::XMMatrixTranspose(pose[i]));
		auto det = DirectX::XMMatrixDeterminant(pose[i]);
		XMStoreFloat4x4(&m_AnimationInverseTransformBufferData[bufferIdx].pose[poseIdx], DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, pose[i])));
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	for (int i = 0; i < nBuffers; i++)
	{

		context->UpdateSubresource1(
			m_AnimationTransformBuffer[i].Get(),
			0,
			NULL,
			&m_AnimationTransformBufferData[i],
			0,
			0,
			0
		);

		context->UpdateSubresource1(
			m_AnimationInverseTransformBuffer[i].Get(),
			0,
			NULL,
			&m_AnimationInverseTransformBufferData[i],
			0,
			0,
			0
		);

		context->VSSetConstantBuffers1(
			2 + i,
			1,
			m_AnimationTransformBuffer[i].GetAddressOf(),
			nullptr,
			nullptr
		);

		context->VSSetConstantBuffers1(
			5 + i,
			1,
			m_AnimationInverseTransformBuffer[i].GetAddressOf(),
			nullptr,
			nullptr
		);

	}
	Base3DRenderer::Render(*animable.m_animatedModel);
}
