#include "pch.h"
#include "Camera.hpp"
#include "..\Common\DirectXHelper.h"

using namespace Windows::Foundation;

DirectX::XMFLOAT4X4 Camera::m_staticViewMatrix;

Camera::Camera(const std::shared_ptr<DX::DeviceResources>& deviceResources,
	float fov,
	DirectX::XMVECTOR position, 
	DirectX::XMVECTOR at, 
	DirectX::XMVECTOR up)
	: m_position(position), m_at(at), m_up(up), m_deviceResources(deviceResources), m_yaw(0), m_pitch(0)
{
	DirectX::XMStoreFloat4x4(
		&m_staticViewMatrix,
		DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(m_position, m_at, m_up))
	);

	this->CreateWindowSizeDependentResources(fov);
	this->updateViewMatrix();
}

void Camera::CreateWindowSizeDependentResources(float fov)
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = fov * DirectX::XM_PI / 180.0f;

	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);

	DirectX::XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
	DirectX::XMMATRIX orientationMatrix = DirectX::XMLoadFloat4x4(&orientation);

	DirectX::XMStoreFloat4x4(
		&m_projectionMatrix,
		DirectX::XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	);
}

DirectX::XMFLOAT4X4 Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

DirectX::XMFLOAT4X4 Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

void Camera::alignWithMouse(const DirectX::Mouse::State& mouseState)
{
	if (mouseState.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{
		m_pitch -= mouseState.y * ROTATION_GAIN;
		m_yaw -= mouseState.x * ROTATION_GAIN;
	}
	
	if (m_pitch > MAX_PITCH)
		m_pitch = MAX_PITCH;

	if (m_pitch < -MAX_PITCH)
		m_pitch = -MAX_PITCH;

	if (m_yaw > 180.0f)
		m_yaw = -180.0f;

	if (m_yaw < -180.0f)
		m_yaw = 180.0f;

	
	float y = sinf(toRadians(m_pitch));
	float r = cosf(toRadians(m_pitch));
	float z = r * cosf(toRadians(m_yaw));
	float x = r * sinf(toRadians(m_yaw));

	m_at = DirectX::XMVectorAdd(m_position, {x, y, z, 0.f});

	updateViewMatrix();
}

float Camera::toRadians(float degrees) const
{
	return degrees / 180.f * DirectX::XM_PI;
}

void Camera::updateViewMatrix()
{
	DirectX::XMStoreFloat4x4(
		&m_viewMatrix,
		DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(m_position, m_at, m_up))
	);

}
