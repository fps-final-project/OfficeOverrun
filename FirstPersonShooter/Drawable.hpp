#pragma once
#include <Mesh.h>
#include <Texture.h>

class Drawable
{
public:
	virtual void Render() = 0;

protected:
	Mesh ObjectMesh;
	Texture ObjectTexture;
};

