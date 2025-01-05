#pragma once
#include "AnimatedAssimpModel.h"


class __declspec(dllexport) Animator
{
public:
	Animator();
	Animator(std::shared_ptr<Animation> animation, std::shared_ptr<Animation> fallback = nullptr, bool wrap = false);
	void updateAnimation(const Joint& rootJoint, const std::map<std::string, BoneInfo>& boneInfoMap, float dt);
	void playAnimation(std::shared_ptr<Animation> animation, float speed = 1.f, bool wrap = false);
	void setFallbackAnimation(std::shared_ptr<Animation> animation);
	void clearFallbackAnimation();
	void calculateTransform(const Joint& data, const std::map<std::string, BoneInfo>& boneInfoMap, DirectX::XMMATRIX parentTransform);
	bool isIdle() const { return m_currentAnimation == m_fallbackAnimation; }

	std::vector<DirectX::XMMATRIX> m_finalBoneMatrices;
	DirectX::XMMATRIX getJointTransform(const Joint& data, float animationTime);
	int getIndex(float animationTime);

private:
	std::shared_ptr<Animation> m_currentAnimation, m_fallbackAnimation;
	float m_deltaTime;
	float m_currentTime;
	float m_animationSpeedMultiplier;
	bool m_wrapAnimation;
	bool hasKeyframeData(const Joint& data);
	float getScaleFactor(float lastTimestamp, float nextTimestamp, float animationTime);
	DirectX::XMMATRIX interpolatePosition(const Joint& data, float animationTime);
	DirectX::XMMATRIX interpolateRotation(const Joint& data, float animationTime);
	DirectX::XMMATRIX interpolateScaling(const Joint& data, float animationTime);
};