#include "pch.h"
#include "Animator.h"

Animator::Animator()
{
    *this = Animator(nullptr, nullptr, false);
}

Animator::Animator(std::shared_ptr<Animation> animation, std::shared_ptr<Animation> fallback, bool wrap)
    : m_wrapAnimation(wrap), m_currentTime(0.f), m_currentAnimation(animation), m_fallbackAnimation(fallback), m_animationSpeedMultiplier(1.f)
{
	m_finalBoneMatrices.reserve(150);
	for (int i = 0; i < 150; i++)
	{
		m_finalBoneMatrices.push_back(DirectX::XMMatrixIdentity());
	}
}

void Animator::updateAnimation(const Joint& rootJoint, const std::map<std::string, BoneInfo>& boneInfoMap, float dt)
{
	m_deltaTime = dt;

    if (!m_currentAnimation && m_fallbackAnimation)
    {
        m_currentAnimation = m_fallbackAnimation;
        m_wrapAnimation = true;
        m_currentTime = 0;
        m_animationSpeedMultiplier = 1.f;
    }

	if (m_currentAnimation)
	{
		m_currentTime += m_currentAnimation->m_TicksPerSecond * dt * m_animationSpeedMultiplier;
        if (!m_wrapAnimation && m_currentTime > m_currentAnimation->m_Duration)
        {
            m_currentAnimation = m_fallbackAnimation;
            m_wrapAnimation = true;
            m_currentTime = 0;
            m_animationSpeedMultiplier = 1.f;
        }
		m_currentTime = fmod(m_currentTime, m_currentAnimation->m_Duration);
		auto ident = DirectX::XMMatrixIdentity();

		calculateTransform(rootJoint, boneInfoMap, ident);
	}
}

void Animator::playAnimation(std::shared_ptr<Animation> animation, float speed, bool wrap)
{
	m_currentAnimation = animation;
	m_currentTime = 0.0f;
    m_wrapAnimation = wrap;
    m_animationSpeedMultiplier = speed;
}

void Animator::setFallbackAnimation(std::shared_ptr<Animation> animation)
{
    m_fallbackAnimation = animation;
}

void Animator::calculateTransform(const Joint& data, const std::map<std::string, BoneInfo>& boneInfoMap, DirectX::XMMATRIX parentTransform)
{
	std::string nodeName = data.name;
	DirectX::XMMATRIX nodeTransform = DirectX::XMLoadFloat4x4(&data.transformation);
    bool hasKeyframeData = this->hasKeyframeData(data);

	if (hasKeyframeData)
	{
		nodeTransform = this->getJointTransform(data, m_currentTime);
	}

	auto globalTransfomation = DirectX::XMMatrixMultiply(nodeTransform, parentTransform);

	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap.at(nodeName).id;
		auto offset = DirectX::XMLoadFloat4x4(&boneInfoMap.at(nodeName).offsetMatrix);
		m_finalBoneMatrices[index] = DirectX::XMMatrixMultiply(offset, globalTransfomation);
	}

	for (int i = 0; i < data.children.size(); i++)
		calculateTransform(data.children[i], boneInfoMap, globalTransfomation);
}

DirectX::XMMATRIX Animator::getJointTransform(const Joint& data, float animationTime)
{
    DirectX::XMMATRIX translation = interpolatePosition(data, animationTime);
    DirectX::XMMATRIX rotation = interpolateRotation(data, animationTime);
    DirectX::XMMATRIX scale = interpolateScaling(data, animationTime);

    auto result = DirectX::XMMatrixMultiply(scale, rotation);
    result = DirectX::XMMatrixMultiply(result, translation);

    return result;
}

int Animator::getIndex(float animationTime)
{
    for (int index = 0; index < m_currentAnimation->frames.size() - 1; ++index)
    {
        if (animationTime < m_currentAnimation->frames[index + 1].timestamp)
            return index;
    }
    assert(0);
}

bool Animator::hasKeyframeData(const Joint& data)
{
    return m_currentAnimation->frames[0].pose.find(data.name) != m_currentAnimation->frames[0].pose.end();
}

float Animator::getScaleFactor(float lastTimestamp, float nextTimestamp, float animationTime)
{
    float midwayLength = animationTime - lastTimestamp;
    float framesDiff = nextTimestamp - lastTimestamp;
    return midwayLength / framesDiff;
}

DirectX::XMMATRIX Animator::interpolatePosition(const Joint& data, float animationTime)
{
    if (1 == m_currentAnimation->frames.size())
    {
        auto p = DirectX::XMLoadFloat3(&m_currentAnimation->frames[0].pose[data.name].position);
        return DirectX::XMMatrixTranslationFromVector(p);
    }

    int p0Index = getIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(m_currentAnimation->frames[p0Index].timestamp, m_currentAnimation->frames[p1Index].timestamp, animationTime);

    auto p0 = DirectX::XMLoadFloat3(&m_currentAnimation->frames[p0Index].pose[data.name].position);
    auto p1 = DirectX::XMLoadFloat3(&m_currentAnimation->frames[p1Index].pose[data.name].position);

    auto finalPosition = DirectX::XMVectorLerp(p0, p1, scaleFactor);

    return DirectX::XMMatrixTranslationFromVector(finalPosition);
}

DirectX::XMMATRIX Animator::interpolateRotation(const Joint& data, float animationTime)
{
    if (1 == m_currentAnimation->frames.size())
    {
        auto r = DirectX::XMLoadFloat4(&m_currentAnimation->frames[0].pose[data.name].orientation);
        r = DirectX::XMVector4Normalize(r);

        return DirectX::XMMatrixRotationQuaternion(r);

    }

    int p0Index = getIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(m_currentAnimation->frames[p0Index].timestamp, m_currentAnimation->frames[p1Index].timestamp, animationTime);

    auto r0 = DirectX::XMLoadFloat4(&m_currentAnimation->frames[p0Index].pose[data.name].orientation);
    auto r1 = DirectX::XMLoadFloat4(&m_currentAnimation->frames[p1Index].pose[data.name].orientation);

    auto finalRotation = DirectX::XMQuaternionSlerp(r0, r1, scaleFactor);
    finalRotation = DirectX::XMVector4Normalize(finalRotation);

    return DirectX::XMMatrixRotationQuaternion(finalRotation);
}

DirectX::XMMATRIX Animator::interpolateScaling(const Joint& data, float animationTime)
{
    if (1 == m_currentAnimation->frames.size())
    {
        auto s = DirectX::XMLoadFloat3(&m_currentAnimation->frames[0].pose[data.name].scale);
        return DirectX::XMMatrixTranslationFromVector(s);
    }

    int p0Index = getIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(m_currentAnimation->frames[p0Index].timestamp, m_currentAnimation->frames[p1Index].timestamp, animationTime);

    auto s0 = DirectX::XMLoadFloat3(&m_currentAnimation->frames[p0Index].pose[data.name].scale);
    auto s1 = DirectX::XMLoadFloat3(&m_currentAnimation->frames[p1Index].pose[data.name].scale);

    auto finalScale = DirectX::XMVectorLerp(s0, s1, scaleFactor);

    return DirectX::XMMatrixScalingFromVector(finalScale);
}
