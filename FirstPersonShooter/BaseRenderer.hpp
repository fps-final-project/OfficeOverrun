#pragma once

#include "Common\DeviceResources.h"
#include "Content\ShaderStructures.h"
#include "Common\StepTimer.h"
#include "AssimpModel.h"
#include "Mesh.h"
#include "Texture.h"
#include "MeshFactory.h"
#include "Animator.h"
#include "Animable.hpp"

#include <chrono>

template<typename VertexShaderBuffer, typename VertexDataType>
class BaseRenderer
{
public:
	BaseRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
		m_loadingComplete(false),
		m_deviceResources(deviceResources)
	{
	}

	virtual void ReleaseDeviceDependentResources()
	{
		m_loadingComplete = false;
		m_vertexShader.Reset();
		m_inputLayout.Reset();
		m_pixelShader.Reset();
		m_VSConstantBuffer.Reset();
	}

	virtual void CreateDeviceDependentResources() = 0;

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
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_VSConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	m_samplerState;

	// System resources for cube geometry.
	VertexShaderBuffer	m_VSConstantBufferData;

	// Variables used with the rendering loop.
	bool	m_loadingComplete;

	virtual void CreateDeviceDependentResources_internal(
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
