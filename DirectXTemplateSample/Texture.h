#pragma once

#include <string>

struct Texture
{
	std::string type;
	std::string path;
	Microsoft::WRL::ComPtr<ID3D11Resource> textureHandle;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

    Texture() = default;

    Texture(const Texture& other)
        : type(other.type),
        path(other.path),
        textureHandle(other.textureHandle),
        shaderResourceView(other.shaderResourceView)
    {
        // ComPtr automatically handles reference counting
    }

    Texture& operator=(const Texture& other)
    {
        if (this != &other)
        {
            type = other.type;
            path = other.path;
            textureHandle = other.textureHandle;
            shaderResourceView = other.shaderResourceView;
        }
        return *this;
    }
};