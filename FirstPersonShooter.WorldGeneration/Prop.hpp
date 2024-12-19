#pragma once

#include <string>
#include <DirectXMath.h>


struct __declspec(dllexport) Prop
{
	std::string name;
	std::string modelPath;
	DirectX::XMFLOAT3 size;
	bool isSolid;
};