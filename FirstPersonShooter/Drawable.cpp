#include "pch.h"
#include "Drawable.hpp"

Drawable::Drawable(std::shared_ptr<AssimpModel> model)
	: m_model(model), m_modelMatrix(DirectX::XMMatrixIdentity())
{
}
