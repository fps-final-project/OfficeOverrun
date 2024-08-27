#pragma once
#include <Mesh.h>
#include <Texture.h>

#include "AssimpModel.h"

class Drawable
{
public:
	virtual void Render() {}
protected:
	std::shared_ptr<AssimpModel> m_model;
};

