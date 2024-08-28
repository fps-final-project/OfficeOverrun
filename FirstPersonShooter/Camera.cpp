#include "pch.h"
#include "Camera.hpp"
#include "..\Common\DirectXHelper.h"

using namespace Windows::Foundation;

Camera::Camera(const std::shared_ptr<DX::DeviceResources>& deviceResources,
	float fov,
	DirectX::XMVECTORF32 position, 
	DirectX::XMVECTORF32 at, 
	DirectX::XMVECTORF32 up)
	: m_position(position), m_at(at), m_up(up), m_deviceResources(deviceResources)
{
	this->CreateWindowSizeDependentResources(95.0f);
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

void Camera::updateViewMatrix()
{
	DirectX::XMStoreFloat4x4(
		&m_viewMatrix,
		DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(m_position, m_at, m_up))
	);
}
