#pragma once

struct AnimatedModelDrawRequest
{
	DirectX::XMMATRIX model;
	Animator m_animator;
	std::shared_ptr<AnimatedAssimpModel> m_animatedModel;
};