#pragma once

#include "Prop.hpp"

struct __declspec(dllexport) PropInstance
{
	std::string name;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 size;
	DirectX::XMFLOAT3 rotation;
	bool isSolid;

	PropInstance(Prop prop, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation)
		: name(prop.name), position(position), size(prop.size), rotation(rotation), isSolid(prop.isSolid)
	{

	}

	PropInstance(Prop prop, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 size)
		: name(prop.name), position(position), size(size), rotation(rotation), isSolid(prop.isSolid)
	{

	}
	
	// debug 
	PropInstance() {}

	PropInstance(const std::string& name, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& size, const DirectX::XMFLOAT3& rotation, bool isSolid)
		: name(name), position(position), size(size), rotation(rotation), isSolid(isSolid)
	{
	}
};