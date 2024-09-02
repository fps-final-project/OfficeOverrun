#pragma once
#include "AnimatedAssimpModel.h"


class Animator
{
public:
	Animator();
	Animator(std::shared_ptr<Animation> animation, std::shared_ptr<Animation> fallback = nullptr, bool wrap = false);
	void updateAnimation(const Joint& rootJoint, const std::map<std::string, BoneInfo>& boneInfoMap, float dt);
	void playAnimation(std::shared_ptr<Animation> animation, bool wrap = false);
	void setFallbackAnimation(std::shared_ptr<Animation> animation);
	void calculateTransform(const Joint& data, const std::map<std::string, BoneInfo>& boneInfoMap, DirectX::XMMATRIX parentTransform);

	std::vector<DirectX::XMMATRIX> getFinalTransformationMatricies(const std::vector<FinalTransformData>& data) const;

	std::shared_ptr<Animation> m_currentAnimation, m_fallbackAnimation;
	float m_currentTime;
	float m_deltaTime;
	DirectX::XMMATRIX getJointTransform(const Joint& data, float animationTime);
	int getIndex(float animationTime);

private:
	bool m_wrapAnimation;
	bool hasKeyframeData(const Joint& data);
	float getScaleFactor(float lastTimestamp, float nextTimestamp, float animationTime);
	DirectX::XMMATRIX interpolatePosition(const Joint& data, float animationTime);
	DirectX::XMMATRIX interpolateRotation(const Joint& data, float animationTime);
	DirectX::XMMATRIX interpolateScaling(const Joint& data, float animationTime);
	std::vector<DirectX::XMMATRIX> m_finalBoneMatrices;
};