#pragma once
#include <DirectXMath.h>
#include <assimp/anim.h>
#include <vector>
#include <string>

struct KeyPosition 
{
	DirectX::XMFLOAT3 position;
	float timestamp;
};

struct KeyRotation
{
	DirectX::XMFLOAT4 orientation;
	float timestamp;
	KeyRotation(aiQuaternion quaternion, float timestamp)
	{
		memcpy(&orientation, &quaternion, 16);
		this->timestamp = timestamp;
	}
};

struct KeyScale
{
	DirectX::XMFLOAT3 scale;
	float timestamp;
};

class Bone
{
private:
	std::vector<KeyPosition> m_positions;
	std::vector<KeyRotation> m_rotations;
	std::vector<KeyScale> m_scales;

public:
	DirectX::XMFLOAT4X4 m_localTransform;
	std::string m_name;
	int m_id;

	Bone(const std::string& name, int id, const aiNodeAnim* channel);
	void update(float animationTime);
	int getPositionIndex(float animationTime);
	int getRotationIndex(float animationTime);
	int getScaleIndex(float animationTime);

private:
	float getScaleFactor(float lastTimestamp, float nextTimestamp, float animationTime);
	DirectX::XMMATRIX interpolatePosition(float animationTime);
	DirectX::XMMATRIX interpolateRotation(float animationTime);
	DirectX::XMMATRIX interpolateScaling(float animationTime);

};