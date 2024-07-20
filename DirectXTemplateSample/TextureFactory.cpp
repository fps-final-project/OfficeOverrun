#include "pch.h"
#include "TextureFactory.h"
#include "iostream"
#include "WICTextureLoader.h"
#include "Common/DirectXHelper.h"


Texture TextureFactory::CreateTextureFromFile(const wchar_t* filename, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	Texture m_texture;
	auto device = deviceResources->GetD3DDevice();
	auto result = DirectX::CreateWICTextureFromFile(device, filename, &m_texture.textureHandle, &m_texture.shaderResourceView);
	DX::ThrowIfFailed(result);
	return m_texture;
}
