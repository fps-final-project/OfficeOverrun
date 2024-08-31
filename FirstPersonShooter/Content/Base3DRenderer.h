#pragma once

#include "BaseRenderer.hpp"
#include "Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "Common\StepTimer.h"
#include "AssimpModel.h"
#include "Mesh.h"
#include "Texture.h"
#include "MeshFactory.h"
#include "Animator.h"
#include "Animable.hpp"

template<typename VertexShaderBuffer, typename PixelShaderBuffer, typename VertexDataType>
class Base3DRenderer : public BaseRenderer<VertexShaderBuffer, VertexDataType>
{
public:
	Base3DRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources)
		: BaseRenderer(deviceResources)
	{
		XMStoreFloat3(&m_PSConstantBufferData.light_pos, { 0.f, 0.f, 0.f, 0.f });
		this->SetClockwiseCulling();
	}

	virtual void CreateDeviceDependentResources() = 0;

	virtual void ReleaseDeviceDependentResources() override
	{
		m_PSConstantBuffer.Reset();
		BaseRenderer<VertexShaderBuffer, VertexDataType>::ReleaseDeviceDependentResources();
	}

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

protected:

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_PSConstantBuffer;
	PixelShaderBuffer m_PSConstantBufferData;

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

	virtual void CreateDeviceDependentResources_internal(
		std::wstring VSpath,
		std::wstring PSpath,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc) override
	{

		CD3D11_BUFFER_DESC PSconstantBufferDesc(sizeof(PixelShaderBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&PSconstantBufferDesc,
				nullptr,
				&m_PSConstantBuffer
			)
		);

		BaseRenderer<VertexShaderBuffer, VertexDataType>::CreateDeviceDependentResources_internal(VSpath, PSpath, inputDesc);
	}

};
