#include "pch.h"
#include "Bone.h"

Bone::Bone(const std::string& name, int id, const aiNodeAnim* channel)
    : m_name(name), m_id(id)
{
    for (int positionIndex = 0; positionIndex < channel->mNumPositionKeys; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        float timeStamp = channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        data.position.x = aiPosition.x;
        data.position.y = aiPosition.y;
        data.position.z = aiPosition.z;
        data.timestamp = timeStamp;
        m_positions.push_back(data);
    }

    for (int rotationIndex = 0; rotationIndex < channel->mNumRotationKeys; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
        m_rotations.push_back(KeyRotation(aiOrientation, timeStamp));
    }

    for (int scaleIndex = 0; scaleIndex < channel->mNumScalingKeys; ++scaleIndex)
    {
        aiVector3D aiScale = channel->mScalingKeys[scaleIndex].mValue;
        float timeStamp = channel->mScalingKeys[scaleIndex].mTime;
        KeyScale data;
        data.scale.x = aiScale.x;
        data.scale.y = aiScale.y;
        data.scale.z = aiScale.z;
        data.timestamp = timeStamp;
        m_scales.push_back(data);
    }
}

void Bone::update(float animationTime)
{
    DirectX::XMMATRIX translation = interpolatePosition(animationTime);
    DirectX::XMMATRIX rotation = interpolateRotation(animationTime);
    DirectX::XMMATRIX scale = interpolateScaling(animationTime);

    auto result = DirectX::XMMatrixMultiply(translation, rotation);
    result = DirectX::XMMatrixMultiply(result, scale);

    DirectX::XMStoreFloat4x4(&m_localTransform, result);
}

int Bone::getPositionIndex(float animationTime)
{
    for (int index = 0; index < m_positions.size() - 1; ++index)
    {
        if (animationTime < m_positions[index + 1].timestamp)
            return index;
    }
    assert(0);
}

int Bone::getRotationIndex(float animationTime)
{
    for (int index = 0; index < m_rotations.size() - 1; ++index)
    {
        if (animationTime < m_rotations[index + 1].timestamp)
            return index;
    }
    assert(0);
}

int Bone::getScaleIndex(float animationTime)
{
    for (int index = 0; index < m_scales.size() - 1; ++index)
    {
        if (animationTime < m_scales[index + 1].timestamp)
            return index;
    }
    assert(0);
}

float Bone::getScaleFactor(float lastTimestamp, float nextTimestamp, float animationTime)
{
    float midwayLength = animationTime - lastTimestamp;
    float framesDiff = nextTimestamp - lastTimestamp;
    return midwayLength / framesDiff;
}

DirectX::XMMATRIX Bone::interpolatePosition(float animationTime)
{
    if (1 == m_positions.size())
    {
        auto p = DirectX::XMLoadFloat3(&m_positions[0].position);
        return DirectX::XMMatrixTranslationFromVector(p);
    }

    int p0Index = getPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(m_positions[p0Index].timestamp, m_positions[p1Index].timestamp, animationTime);

    auto p0 = DirectX::XMLoadFloat3(&m_positions[p0Index].position);
    auto p1 = DirectX::XMLoadFloat3(&m_positions[p1Index].position);

    auto finalPosition = DirectX::XMVectorLerp(p0, p1, scaleFactor);

    return DirectX::XMMatrixTranslationFromVector(finalPosition);
}

DirectX::XMMATRIX Bone::interpolateRotation(float animationTime)
{
    if (1 == m_rotations.size())
    {
        auto r = DirectX::XMLoadFloat4(&m_rotations[0].orientation);
        r = DirectX::XMVector4Normalize(r);

        return DirectX::XMMatrixRotationQuaternion(r);

    }

    int p0Index = getRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(m_rotations[p0Index].timestamp, m_rotations[p1Index].timestamp, animationTime);

    auto r0 = DirectX::XMLoadFloat4(&m_rotations[p0Index].orientation);
    auto r1 = DirectX::XMLoadFloat4(&m_rotations[p1Index].orientation);

    auto finalRotation = DirectX::XMQuaternionSlerp(r0, r1, scaleFactor);
    finalRotation = DirectX::XMVector4Normalize(finalRotation);

    return DirectX::XMMatrixRotationQuaternion(finalRotation);
}

DirectX::XMMATRIX Bone::interpolateScaling(float animationTime)
{
    if (1 == m_scales.size())
    {
        auto s = DirectX::XMLoadFloat3(&m_scales[0].scale);
        return DirectX::XMMatrixTranslationFromVector(s);
    }

    int p0Index = getScaleIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(m_scales[p0Index].timestamp, m_scales[p1Index].timestamp, animationTime);

    auto s0 = DirectX::XMLoadFloat3(&m_scales[p0Index].scale);
    auto s1 = DirectX::XMLoadFloat3(&m_scales[p1Index].scale);

    auto finalScale = DirectX::XMVectorLerp(s0, s1, scaleFactor);

    return DirectX::XMMatrixScalingFromVector(finalScale);
}
