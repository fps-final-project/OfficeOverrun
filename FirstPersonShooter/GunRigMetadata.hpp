#pragma once
#include <string>
#include <DirectXMath.h>

struct GunRigMetadata
{
	std::string name;
	float shootingAnimationSpeedup;
	DirectX::XMFLOAT3 gunOffset;
	DirectX::XMFLOAT3 barrelOffset;
	DirectX::XMFLOAT3 rigOffset;
	static GunRigMetadata loadFromFile(const std::string& path);
};