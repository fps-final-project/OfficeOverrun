#include "pch.h"
#include "ModelRenderer.hpp"

ModelRenderer::ModelRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: Base3DRenderer(deviceResources)
{
	this->CreateDeviceDependentResources();
}

void ModelRenderer::CreateDeviceDependentResources()
{
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDesc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 3 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 5 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	this->CreateDeviceDependentResources_internal(L"SampleVertexShader.cso", L"SamplePixelShader.cso", vertexDesc);
}

void ModelRenderer::Render(const Drawable& drawable)
{
	if (!m_loadingComplete)
	{
		return;
	}

	DirectX::XMStoreFloat4x4(&m_VSConstantBufferData.model, DirectX::XMMatrixTranspose(drawable.m_modelMatrix));
	auto det = DirectX::XMMatrixDeterminant(drawable.m_modelMatrix);
	XMStoreFloat4x4(&m_VSConstantBufferData.inv_model, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, drawable.m_modelMatrix)));
	Base3DRenderer::Render(*drawable.m_model);
}
