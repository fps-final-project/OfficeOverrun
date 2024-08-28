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

using namespace FirstPersonShooter;

class Base3DRenderer
{
public:
	Base3DRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	void CreateDeviceDependentResources();
	void ReleaseDeviceDependentResources();
	void Render(const Mesh& m);
	void Render(const AssimpModel& m);
	void Render(const Animable& animable);

	void setProjectionMatrix(DirectX::XMFLOAT4X4 projection);
	void setViewMatrix(DirectX::XMFLOAT4X4 view);

	void SetClockwiseCulling();


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
	AnimationConstantBuffer	m_VSConstantBufferData;
	LightingConstantBuffer m_PSConstantBufferData;

	// Variables used with the rendering loop.
	bool	m_loadingComplete;
};
