#pragma once

#include "Common\DeviceResources.h"
#include <memory>

class Camera
{
public:
	Camera(const std::shared_ptr<DX::DeviceResources>& deviceResources,
		float fov = 95.0f,
		DirectX::XMVECTORF32 position = { 0.0f, 0.0f, 0.0f, 0.0f },
		DirectX::XMVECTORF32 at = { 0.0f, 0.0f, 1.0f, 0.0f },
		DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f });

	void CreateWindowSizeDependentResources(float fov);

	DirectX::XMFLOAT4X4 getProjectionMatrix() const;
	DirectX::XMFLOAT4X4 getViewMatrix() const;

private:
	std::shared_ptr<DX::DeviceResources> m_deviceResources;

	DirectX::XMFLOAT4X4 m_viewMatrix;
	DirectX::XMFLOAT4X4 m_projectionMatrix;
	DirectX::XMVECTORF32 m_position;
	DirectX::XMVECTORF32 m_at;
	DirectX::XMVECTORF32 m_up;

	void updateViewMatrix();
};