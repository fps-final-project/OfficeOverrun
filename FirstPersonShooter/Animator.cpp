#include "pch.h"
#include "Animator.h"

Animator::Animator(Animation* animation)
{
	m_CurrentTime = 0.0f;
	m_CurrentAnimation = animation;

	m_FinalBoneMatrices.reserve(55);
	for (int i = 0; i < 55; i++)
	{
		auto ident = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT4X4 ident_store;
		DirectX::XMStoreFloat4x4(&ident_store, ident);
		m_FinalBoneMatrices.push_back(ident_store);
	}
}

void Animator::updateAnimation(float dt)
{
	m_DeltaTime = dt;
	if (m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->m_TicksPerSecond * dt;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->m_Duration);
		auto ident = DirectX::XMMatrixIdentity();

		calculateBoneTransform(&m_CurrentAnimation->m_RootNode, ident);
	}
}

void Animator::playAnimation(Animation* animation)
{
	m_CurrentAnimation = animation;
	m_CurrentTime = 0.0f;
}

void Animator::calculateBoneTransform(const AssimpNodeData* data, DirectX::XMMATRIX parentTransform)
{
	std::string nodeName = data->name;
	DirectX::XMMATRIX nodeTransform = DirectX::XMLoadFloat4x4(&data->transformation);

	Bone* bone = m_CurrentAnimation->getBone(nodeName);

	if (bone)
	{
		bone->update(m_CurrentTime);
		nodeTransform = DirectX::XMLoadFloat4x4(&bone->m_localTransform);
	}

	auto globalTransfomation = DirectX::XMMatrixMultiply(nodeTransform, parentTransform);

	auto boneInfoMap = m_CurrentAnimation->m_BoneInfoMap;
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		auto offset = DirectX::XMLoadFloat4x4(&boneInfoMap[nodeName].offsetMatrix);
		DirectX::XMStoreFloat4x4(&m_FinalBoneMatrices[index], DirectX::XMMatrixMultiply(offset, globalTransfomation));
	}

	for (int i = 0; i < data->children.size(); i++)
		calculateBoneTransform(&data->children[i], globalTransfomation);






}
