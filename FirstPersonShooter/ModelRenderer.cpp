#include "pch.h"
#include "ModelRenderer.hpp"

ModelRenderer::ModelRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: Base3DRenderer(deviceResources)
{
	this->CreateDeviceDependentResources();
}

ModelRenderer::~ModelRenderer()
{
	this->ReleaseDeviceDependentResources();
}

void ModelRenderer::CreateDeviceDependentResources()
{
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDesc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 3 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 5 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	this->CreateDeviceDependentResources_internal(L"FirstPersonShooter_Rendering\\SampleVertexShader.cso", L"FirstPersonShooter_Rendering\\PhongPixelShader.cso", vertexDesc);
}

void ModelRenderer::Render(const Model& model, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotation)
{
	if (!m_loadingComplete)
	{
		return;
	}

	auto modelMatrix = getModelMatrix(position, rotation, scale);

	DirectX::XMStoreFloat4x4(&m_VSConstantBufferData.model, DirectX::XMMatrixTranspose(modelMatrix));
	auto det = DirectX::XMMatrixDeterminant(modelMatrix);
	XMStoreFloat4x4(&m_VSConstantBufferData.inv_model, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, modelMatrix)));
	Base3DRenderer::Render(model);
}

