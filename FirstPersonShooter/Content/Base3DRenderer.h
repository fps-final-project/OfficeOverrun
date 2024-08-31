#pragma once

#include "Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "Common\StepTimer.h"
#include "AssimpModel.h"
#include "Mesh.h"
#include "Texture.h"
#include "MeshFactory.h"
#include "Animator.h"
#include "Animable.hpp"

#include <chrono>

template<typename VertexShaderBuffer, typename PixelShaderBuffer, typename VertexDataType>
class Base3DRenderer
{
public:
	Base3DRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
		m_loadingComplete(false),
		m_deviceResources(deviceResources)
	{
		XMStoreFloat3(&m_PSConstantBufferData.light_pos, { 0.f, 0.f, 0.f, 0.f });
		this->SetClockwiseCulling();
	}

	void ReleaseDeviceDependentResources()
	{
		m_loadingComplete = false;
		m_vertexShader.Reset();
		m_inputLayout.Reset();
		m_pixelShader.Reset();
		m_VSConstantBuffer.Reset();
		m_PSConstantBuffer.Reset();
	}

	virtual void CreateDeviceDependentResources() = 0;

	void Render(const Mesh& m)
	{
		auto context = m_deviceResources->GetD3DDeviceContext();

		// We only handle diffuse textures for now (actual color textures)
		context->PSSetShaderResources(0, 1, m.textures[0].shaderResourceView.GetAddressOf());

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

		context->UpdateSubresource1(
			m_PSConstantBuffer.Get(),
			0,
			NULL,
			&m_PSConstantBufferData,
			0,
			0,
			0
		);

		// Each vertex is one instance of the VertexData struct.
		UINT stride = sizeof(VertexDataType);
		UINT offset = 0;

		context->IASetVertexBuffers(
			0,
			1,
			m.vertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		context->IASetIndexBuffer(
			m.indexBuffer.Get(),
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

		context->PSSetConstantBuffers1(
			1,
			1,
			m_PSConstantBuffer.GetAddressOf(),
			nullptr,
			nullptr
		);


		// Draw the objects.
		context->DrawIndexed(
			m.indexCount,
			0,
			0
		);
	}

	void Render(const AssimpModel& m)
	{
		for (const auto& m_mesh : m.meshes)
		{
			this->Render(m_mesh);
		}
	}

	void setProjectionMatrix(const DirectX::XMFLOAT4X4& projection)
	{
		m_VSConstantBufferData.projection = projection;
	}

	void setViewMatrix(const DirectX::XMFLOAT4X4& view)
	{
		m_VSConstantBufferData.view = view;
	}

	void SetClockwiseCulling()
	{
		ID3D11Device* device = m_deviceResources->GetD3DDevice();
		ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

		// Define the rasterizer state description
		D3D11_RASTERIZER_DESC rasterDesc = {};
		rasterDesc.FillMode = D3D11_FILL_SOLID;            // Use solid fill mode
		rasterDesc.CullMode = D3D11_CULL_FRONT;             // Cull back faces
		rasterDesc.FrontCounterClockwise = FALSE;          // Clockwise faces are front faces
		rasterDesc.DepthClipEnable = TRUE;                 // Enable depth clipping

		// Create the rasterizer state
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
		HRESULT hr = device->CreateRasterizerState(&rasterDesc, rasterizerState.GetAddressOf());
		if (FAILED(hr))
		{
			// Handle error (e.g., throw an exception, log the error, etc.)
			return;
		}

		// Set the rasterizer state
		context->RSSetState(rasterizerState.Get());
	}

	// Call this function just before the first render calls
	void use()
	{
		auto context = m_deviceResources->GetD3DDeviceContext();

		// Attach vertex shader.
		context->VSSetShader(
			m_vertexShader.Get(),
			nullptr,
			0
		);

		// Attach pixel shader.
		context->PSSetShader(
			m_pixelShader.Get(),
			nullptr,
			0
		);

		// set texture sampler
		context->PSSetSamplers(
			0,
			1,
			m_samplerState.GetAddressOf()
		);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetInputLayout(m_inputLayout.Get());
	}

protected:
	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;

	// Direct3D resources for cube geometry.
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_VSConstantBuffer, m_PSConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	m_samplerState;

	// System resources for cube geometry.
	VertexShaderBuffer	m_VSConstantBufferData;
	PixelShaderBuffer m_PSConstantBufferData;

	// Variables used with the rendering loop.
	bool	m_loadingComplete;

	void CreateDeviceDependentResources_internal(
		std::wstring VSpath,
		std::wstring PSpath,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc)
	{
		// Load shaders asynchronously.
		auto loadVSTask = DX::ReadDataAsync(VSpath);
		auto loadPSTask = DX::ReadDataAsync(PSpath);

		// After the vertex shader file is loaded, create the shader and input layout.
		auto createVSTask = loadVSTask.then([&, this](const std::vector<byte>& fileData) {
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateVertexShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_vertexShader
				)
			);

			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateInputLayout(
					inputDesc.data(),
					inputDesc.size(),
					&fileData[0],
					fileData.size(),
					&m_inputLayout
				)
			);
			});

		// After the pixel shader file is loaded, create the shader and constant buffer.
		auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreatePixelShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_pixelShader
				)
			);

			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(VertexShaderBuffer), D3D11_BIND_CONSTANT_BUFFER);
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateBuffer(
					&constantBufferDesc,
					nullptr,
					&m_VSConstantBuffer
				)
			);


			CD3D11_BUFFER_DESC PSconstantBufferDesc(sizeof(PixelShaderBuffer), D3D11_BIND_CONSTANT_BUFFER);
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateBuffer(
					&PSconstantBufferDesc,
					nullptr,
					&m_PSConstantBuffer
				)
			);
			});

		auto createSamplerTask = (createPSTask && createVSTask).then([this]() {
			auto device = m_deviceResources->GetD3DDevice();
			D3D11_SAMPLER_DESC ImageSamplerDesc = {};

			ImageSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			ImageSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			ImageSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			ImageSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			ImageSamplerDesc.MipLODBias = 0.0f;
			ImageSamplerDesc.MaxAnisotropy = 1;
			ImageSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			ImageSamplerDesc.BorderColor[0] = 1.0f;
			ImageSamplerDesc.BorderColor[1] = 1.0f;
			ImageSamplerDesc.BorderColor[2] = 1.0f;
			ImageSamplerDesc.BorderColor[3] = 1.0f;
			ImageSamplerDesc.MinLOD = -FLT_MAX;
			ImageSamplerDesc.MaxLOD = FLT_MAX;

			auto Result = device->CreateSamplerState(&ImageSamplerDesc,
				&m_samplerState);

			});
		// Once the cube is loaded, the object is ready to be rendered.
		createSamplerTask.then([this]() {
			m_loadingComplete = true;
			});
	}
};