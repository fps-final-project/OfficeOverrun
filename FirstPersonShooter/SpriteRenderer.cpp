#include "pch.h"
#include "SpriteRenderer.hpp"
#include "MeshFactory.h"

using namespace Windows::Foundation;

SpriteRenderer::SpriteRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: BaseRenderer(deviceResources)
{
	this->CreateDeviceDependentResources();
	this->CreateWindowSizeDependentResources();
}

void SpriteRenderer::CreateDeviceDependentResources()
{
	static std::vector<VertexData2D> cubeVertices =
	{
		{ DirectX::XMFLOAT2(-0.5f,  0.5f), DirectX::XMFLOAT2(0.0f, 0.0f)},
		{ DirectX::XMFLOAT2(-0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f)},
		{ DirectX::XMFLOAT2(0.5f,  -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)},
		{ DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f)}
	};

	static std::vector<unsigned short> cubeIndices =
	{
		0, 1, 2, 2, 3, 0,
	};

	this->m_spriteMesh = MeshFactory<VertexData2D>::createMesh(cubeVertices, cubeIndices, std::vector<std::shared_ptr<Texture>>(), m_deviceResources);


	static const std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDesc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 2 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	this->CreateDeviceDependentResources_internal(L"HUDVertexShader.cso", L"HUDPixelShader.cso", vertexDesc);
}

void SpriteRenderer::Render(const std::shared_ptr<Texture>& texture, int x, int y, int sizeX, int sizeY)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	DirectX::XMStoreFloat4x4(&m_VSConstantBufferData.model,
		DirectX::XMMatrixTranslation(x, y, 1.f) 
		* DirectX::XMMatrixScaling(sizeX, sizeY, 1.f));

	// We only handle diffuse textures for now (actual color textures)
	context->PSSetShaderResources(0, 1, texture->shaderResourceView.GetAddressOf());

	// it has to be called per Entity cause we dynamically set animation offsets in the buffer, 
	// maybe we can optimize this if it is an issue
	context->UpdateSubresource1(
		m_VSConstantBuffer.Get(),
		0,
		NULL,
		&m_VSConstantBufferData,
		0,
		0,
		0
	);

	// Each vertex is one instance of the VertexData struct.
	UINT stride = sizeof(VertexData2D);
	UINT offset = 0;

	context->IASetVertexBuffers(
		0,
		1,
		m_spriteMesh.vertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	context->IASetIndexBuffer(
		m_spriteMesh.indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
	);


	// Send the constant buffer to the graphics device. Same as before can be optimized
	context->VSSetConstantBuffers1(
		0,
		1,
		m_VSConstantBuffer.GetAddressOf(),
		nullptr,
		nullptr
	);

	// Draw the objects.
	context->DrawIndexed(
		m_spriteMesh.indexCount,
		0,
		0
	);
}

void SpriteRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	DirectX::XMStoreFloat4x4(&m_VSConstantBufferData.projection, 
		DirectX::XMMatrixOrthographicOffCenterLH(0.f, outputSize.Width, outputSize.Height, 0.f, 0.1f, 100.f));
}
