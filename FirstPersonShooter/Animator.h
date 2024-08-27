#pragma once
#include "AnimatedAssimpModel.h"


class Animator
{
public:
	Animator();
	Animator(Animation* animation);
	void updateAnimation(const Joint& rootJoint, const std::map<std::string, BoneInfo>& boneInfoMap, float dt);
	void playAnimation(Animation* animation);
	void calculateTransform(const Joint& data, const std::map<std::string, BoneInfo>& boneInfoMap, DirectX::XMMATRIX parentTransform);

	std::vector<DirectX::XMFLOAT4X4> m_FinalBoneMatrices;
	Animation* m_CurrentAnimation;
	float m_CurrentTime;
	float m_DeltaTime;
	DirectX::XMMATRIX getJointTransform(const Joint& data, float animationTime);
	int getIndex(float animationTime);

private:
	bool hasKeyframeData(const Joint& data);
	float getScaleFactor(float lastTimestamp, float nextTimestamp, float animationTime);
	DirectX::XMMATRIX interpolatePosition(const Joint& data, float animationTime);
	DirectX::XMMATRIX interpolateRotation(const Joint& data, float animationTime);
	DirectX::XMMATRIX interpolateScaling(const Joint& data, float animationTime);
};