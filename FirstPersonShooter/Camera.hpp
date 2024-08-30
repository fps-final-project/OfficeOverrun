#pragma once

#include "Common\DeviceResources.h"
#include <memory>
#include <Mouse.h>

class Camera
{
public:
	Camera(const std::shared_ptr<DX::DeviceResources>& deviceResources,
		float fov = 95.0f,
		DirectX::XMVECTOR position = { 0.0f, 0.0f, 0.0f, 0.0f },
		DirectX::XMVECTOR at = { 0.0f, 0.0f, 1.0f, 0.0f },
		DirectX::XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f });

	void CreateWindowSizeDependentResources(float fov);

	DirectX::XMFLOAT4X4 getProjectionMatrix() const;
	DirectX::XMFLOAT4X4 getViewMatrix() const;

	void alignWithMouse(const DirectX::Mouse::State& mouseState);

	static DirectX::XMFLOAT4X4 m_staticViewMatrix;

private:
	// TODO:
	// camera speed should be the same regardless of fps
	const float ROTATION_GAIN = 0.1f;
	const float MAX_PITCH = 89.0f;

	std::shared_ptr<DX::DeviceResources> m_deviceResources;

	DirectX::XMFLOAT4X4 m_viewMatrix;
	DirectX::XMFLOAT4X4 m_projectionMatrix;
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_at;
	DirectX::XMVECTOR m_up;

	float m_yaw;
	float m_pitch;

	float toRadians(float degrees);

	void updateViewMatrix();
};