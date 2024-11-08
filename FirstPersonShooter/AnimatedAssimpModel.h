#pragma once

#include "AssimpModel.h"
#include "BoneInfo.h"

#include <map>
#include <optional>

struct Joint
{
	int id;
	std::string name;
	DirectX::XMFLOAT4X4 transformation;
	std::vector<Joint> children;
};

struct JointTransform
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 orientation;
	DirectX::XMFLOAT3 scale;
};

struct KeyFrame
{
	std::map<std::string, JointTransform> pose;
	float timestamp;
};

struct Animation
{
	std::string name;
	std::vector<KeyFrame> frames;
	double m_TicksPerSecond;
	double m_Duration;

};


struct FinalTransformData
{
	int boneIds[MAX_BONE_INFLUENCE];
	float weights[MAX_BONE_INFLUENCE];

	FinalTransformData()
	{
		for (int i = 0; i < 4; i++)
		{
			boneIds[i] = -1;
			weights[i] = 0.0f;
		}
	}
};

struct AnimatedModel : public Model
{
	std::vector<FinalTransformData> m_transformData;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
	Joint m_rootJoint;
	std::map<std::string, std::shared_ptr<Animation>> m_animations;
	int m_boneCounter;

};