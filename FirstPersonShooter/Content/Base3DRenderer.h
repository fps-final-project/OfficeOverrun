#pragma once

#include <dxgi.h>
#include "Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "AssimpModel.h"
#include "Mesh.h"
#include "Texture.h"
#include "MeshFactory.h"
#include "LightingData.hpp"

template<typename VertexShaderBuffer, typename VertexDataType>
class Base3DRenderer
{
public:
	Base3DRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
		m_loadingComplete(false),
		m_deviceResources(deviceResources)
	{
		this->SetClockwiseCulling();
	}

	virtual ~Base3DRenderer() 
	{
		this->ReleaseDeviceDependentResources();
	}

	virtual void ReleaseDeviceDependentResources()
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
		if (m.textures.size() > 0)
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

	void ClearStencilBuffer()
	{
		m_deviceResources
			->GetD3DDeviceContext()
			->ClearDepthStencilView(
				m_deviceResources->GetDepthStencilView(),
				D3D11_CLEAR_STENCIL,
				1.0f,
				0
			);
	}

	void SetStencilBufferReferenceValue(UINT8 value)
	{
		m_deviceResources
			->GetD3DDeviceContext()
			->OMSetDepthStencilState(m_deviceResources->GetDepthStencilState(), value);
	}

	UINT8 GetStencilBufferValue()
	{
		auto depthStencilTexture = m_deviceResources->GetDepthStencilTexture();

		m_deviceResources
			->GetD3DDeviceContext()
			->CopyResource(m_stagingTexture.Get(), depthStencilTexture);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = m_deviceResources
			->GetD3DDeviceContext()
			->Map(m_stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);

		if (SUCCEEDED(hr))
		{
			BYTE* pData = reinterpret_cast<BYTE*>(mappedResource.pData);

			// Calculate the address of the middle pixel
			BYTE* row = pData + middlePixel.second * mappedResource.RowPitch;
			UINT32* pixel = reinterpret_cast<UINT32*>(row + middlePixel.first * sizeof(UINT32));

			// Extract the stencil value (the upper 8 bits)
			UINT8 stencilValue = static_cast<UINT8>((*pixel >> 24));

			// Get the size of the data
			m_deviceResources
				->GetD3DDeviceContext()
				->Unmap(m_stagingTexture.Get(), 0);

			return stencilValue;
		}
		return 0;
	}

	void Render(const Model& m)
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

	void setCameraPosition(const DirectX::XMFLOAT3& pos)
	{
		DirectX::XMStoreFloat3(&m_PSConstantBufferData.camera_pos, DirectX::XMLoadFloat3(&pos));
	}

	void setFlashlight(const DirectX::XMFLOAT3& direction, const float cutoff)
	{
		m_PSConstantBufferData.flashlight_dir = direction;
		m_PSConstantBufferData.flashlight_cutoffAngle = cutoff;
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

		context->OMSetDepthStencilState(m_deviceResources->GetDepthStencilState(), 1);

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

	void setLighting(const LightingData& data)
	{
		int nlights = min(data.lightPositions.size(), LightingConstantBuffer::MAX_LIGHTS);
		for (int i = 0; i < nlights; i++)
		{
			m_PSConstantBufferData.light_pos[i] = data.lightPositions[i];
		}

		m_PSConstantBufferData.nlights = nlights;
	}

	void setFullyVisible()
	{
		m_PSConstantBufferData.fully_visible = true;
	}

	void clearFullyVisible()
	{
		m_PSConstantBufferData.fully_visible = false;
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
	Microsoft::WRL::ComPtr<ID3D11Texture2D>		m_stagingTexture;
	
	// System resources for cube geometry.
	VertexShaderBuffer	m_VSConstantBufferData;
	LightingConstantBuffer m_PSConstantBufferData;

	// Variables used with the rendering loop.
	bool	m_loadingComplete;

	std::pair<UINT, UINT> middlePixel;

	void CreateDeviceDependentResources_internal(
		std::wstring VSpath,
		std::wstring PSpath,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc)
	{

		auto depthStencilTexture = m_deviceResources->GetDepthStencilTexture();

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilTexture->GetDesc(&depthStencilDesc);

		// Modify the description for the staging resource
		D3D11_TEXTURE2D_DESC stagingDesc = depthStencilDesc;
		stagingDesc.BindFlags = 0; // No binding flags
		stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU read access
		stagingDesc.Usage = D3D11_USAGE_STAGING; // Usage staging
		stagingDesc.MiscFlags = 0; // No miscellaneous flags

		// Create the staging resource
		DX::ThrowIfFailed(
			m_deviceResources
				->GetD3DDevice()
				->CreateTexture2D(
					&stagingDesc, 
					nullptr, 
					m_stagingTexture.GetAddressOf()
				)
			);


		UINT middleX = stagingDesc.Width / 2;
		UINT middleY = stagingDesc.Height / 2;

		middlePixel = std::make_pair(middleX, middleY);


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


			CD3D11_BUFFER_DESC PSconstantBufferDesc(sizeof(LightingConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
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