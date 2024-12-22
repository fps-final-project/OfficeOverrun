#pragma once

#include <DirectXCollision.h>
#include <DirectXMath.h>

class __declspec(dllexport) Hittable
{
public:
	Hittable(DirectX::BoundingBox box, DirectX::XMFLOAT3 position);
	virtual ~Hittable() {}

	bool Hit(const Hittable& other);
protected:
	DirectX::BoundingBox boundingBox;
	DirectX::XMFLOAT3 position;
};

