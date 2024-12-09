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
	int damage;
	int clipSize;
	static GunRigMetadata loadFromFile(const std::string& path);
};