#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "..\Common\DirectXHelper.h"

#include <vector>
#include "MeshFactory.h"
#include "TextureFactory.h"
#include "AssimpModelLoader.h"

using namespace FirstPersonShooter;

using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_VSConstantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_VSConstantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	XMStoreFloat3(&m_PSConstantBufferData.camera_pos, eye);
	XMStoreFloat3(&m_PSConstantBufferData.light_pos, XMVECTORF32{0, 0.7f, 1.5f, 0});


}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		Rotate(radians);
	}
}

// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::Rotate(float radians)
{
	// Prepare to pass the updated model matrix to the shader
	auto modelMatrix = XMMatrixScaling(0.2, 0.2, 0.2) * XMMatrixRotationY(radians);


	XMStoreFloat4x4(&m_VSConstantBufferData.model, XMMatrixTranspose(modelMatrix));
	auto det = XMMatrixDeterminant(modelMatrix);
	XMStoreFloat4x4(&m_VSConstantBufferData.inv_model, XMMatrixTranspose(XMMatrixInverse(&det, modelMatrix)));
}

void Sample3DSceneRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		//Rotate(radians);
		Rotate(0);
		
	}
}

void Sample3DSceneRenderer::StopTracking()
{
	m_tracking = false;
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render()
{

	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	this->Render(*m_assimpModel);
}

void FirstPersonShooter::Sample3DSceneRenderer::Render(const Mesh& m)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	// We only handle diffuse textures for now (actual color textures)
	context->PSSetShaderResources(0, 1, m.textures[0].shaderResourceView.GetAddressOf());

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
	UINT stride = sizeof(VertexData);
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

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
	);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
	);

	// Send the constant buffer to the graphics device.
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

	// set texture sampler
	context->PSSetSamplers(
		0,
		1,
		m_samplerState.GetAddressOf());

	// Draw the objects.
	context->DrawIndexed(
		m.indexCount,
		0,
		0
	);

}

void FirstPersonShooter::Sample3DSceneRenderer::Render(const AssimpModel& m)
{
	for (const auto& m_mesh : m.meshes)
	{	
		this->Render(m_mesh);
	}
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
				)
			);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc [] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 3 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 5 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
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

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer) , D3D11_BIND_CONSTANT_BUFFER);
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

	// Once both shaders are loaded, create the m_mesh.
	auto createCubeTask = (createPSTask && createVSTask).then([this] () {

		static std::vector<VertexData> cubeVertices = 
		{
			// Front face
		{ XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(0.5f,  -0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

		// Back face
		{ XMFLOAT3(0.5f,  0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)  },
		{ XMFLOAT3(-0.5f,  -0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)  },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)  },

		// Left face
		{ XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		// Right face
		{ XMFLOAT3(0.5f,  0.5f,  -0.5f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f,  -0.5f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)  },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)  },
		{ XMFLOAT3(0.5f,  0.5f, 0.5f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)  },

		// Top face
		{ XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)  },
		{ XMFLOAT3(-0.5f,  0.5f,  -0.5f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)  },
		{ XMFLOAT3(0.5f,  0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)  },
		{ XMFLOAT3(0.5f,  0.5f, 0.5f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)  },

		// Bottom face
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)  },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)   },
		{ XMFLOAT3(0.5f, -0.5f,  0.5f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)   },
		{ XMFLOAT3(0.5f, -0.5f,  -0.5f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)   }
		};

		static std::vector<unsigned short> cubeIndices =
		{
			// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20
		};


		this->m_mesh = MeshFactory::createMesh(cubeVertices, cubeIndices, std::vector<std::shared_ptr<Texture>>(), m_deviceResources);
		this->m_texture = TextureFactory::CreateTextureFromFile(L"Assets\\AK-47\\textures\\AK_Base_color.png", m_deviceResources);
		this->m_assimpModel = std::make_unique<AssimpModel>(AssimpModelLoader::createModelFromFile("Assets\\AK-47\\AK47NoSubdiv.obj", m_deviceResources));
		//this->m_assimpModel = std::unique_ptr<AssimpModel>(new AssimpModel("Assets\\cube\\cube.obj", m_deviceResources));
	});

	auto createSamplerTask = createCubeTask.then([this]() {
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
	createSamplerTask.then([this] () {
		m_loadingComplete = true;
	});
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_VSConstantBuffer.Reset();
	m_PSConstantBuffer.Reset();
}


void Sample3DSceneRenderer::SetClockwiseCulling()
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