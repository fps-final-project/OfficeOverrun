#pragma once

#include "Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "Common\StepTimer.h"
#include "AssimpModel.h"
#include "Mesh.h"
#include "Texture.h"
#include "MeshFactory.h"
#include "Animator.h"
#include "AnimatedModelDrawRequest.hpp"
#include "Animable.hpp"

#include <chrono>

using namespace FirstPersonShooter;
// This sample renderer instantiates a basic rendering pipeline.
class Base3DRenderer
{
public:
	Base3DRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void ReleaseDeviceDependentResources();
	void Render(const Mesh& m);
	void Render(const AssimpModel& m);
	void Render(const AnimatedModelDrawRequest& request);
	void Render(const Animable& animable);

	void SetClockwiseCulling();

private:

private:
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

	uint32	m_indexCount;
	std::chrono::time_point<std::chrono::high_resolution_clock> lastFrame;

	// Variables used with the rendering loop.
	bool	m_loadingComplete;
};
