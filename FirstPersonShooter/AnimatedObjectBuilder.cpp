#include "pch.h"
#include "AnimatedObjectBuilder.hpp"

AnimatedObjectBuilder& AnimatedObjectBuilder::WithNewObject(std::shared_ptr<AnimatedModel> model)
{
    m_object.reset();
    m_object = std::make_shared<AnimatedObject>(model);
    return *this;
}

AnimatedObjectBuilder& AnimatedObjectBuilder::WithPosition(DirectX::XMFLOAT3 pos)
{
    m_object->position = pos;
    return *this;
}

AnimatedObjectBuilder& AnimatedObjectBuilder::WithRotation(DirectX::XMFLOAT3 rot)
{
    m_object->rotation = rot;
    return *this;
}

AnimatedObjectBuilder& AnimatedObjectBuilder::WithVelocity(DirectX::XMFLOAT3 vel)
{
    m_object->velocity = vel;
    return *this;
}

AnimatedObjectBuilder& AnimatedObjectBuilder::WithSize(DirectX::XMFLOAT3 size)
{
    m_object->size = size;
    return *this;
}

AnimatedObjectBuilder& AnimatedObjectBuilder::WithFallbackAnimation(std::string animationName)
{
    m_object->setFallbackAnimation(animationName);
    return *this;
}

std::shared_ptr<AnimatedObject>& AnimatedObjectBuilder::Build()
{
    return m_object;
}
