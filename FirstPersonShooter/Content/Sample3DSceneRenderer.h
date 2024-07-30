#pragma once

#include "Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "Common\StepTimer.h"
#include "AssimpModel.h"
#include "Mesh.h"
#include "Texture.h"
#include "MeshFactory.h"
#include "Animator.h"

#include <chrono>

namespace FirstPersonShooter
{
	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer
	{
	public:
		Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();
		void Render(const Mesh& m);
		void Render(const AssimpModel& m);
		void StartTracking();
		void TrackingUpdate(float positionX);
		void StopTracking();
		bool IsTracking() { return m_tracking; }

		void SetClockwiseCulling();

	private:
		void Rotate(float radians, double totalSeconds);

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		Mesh m_mesh;
		std::shared_ptr<AnimatedAssimpModel> m_assimpModel;
		std::unique_ptr<Animation> m_animation;
		std::unique_ptr<Animator> m_animator;

		Texture m_texture;

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
		float	m_degreesPerSecond;
		bool	m_tracking;
	};
}

