#pragma once
#include <Mesh.h>
#include <Texture.h>

#include "AssimpModel.h"

class Drawable
{
public:
	Drawable(std::shared_ptr<AssimpModel> model);
	virtual ~Drawable() {}
	//virtual void Render() {}
	friend class ModelRenderer;
protected:
	std::shared_ptr<AssimpModel> m_model;
	DirectX::XMMATRIX m_modelMatrix;
};

