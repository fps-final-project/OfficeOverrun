#pragma once
#include "BaseRenderer.hpp"
#include "Mesh.h"


class SpriteRenderer : public BaseRenderer<VertexShaderData2D, VertexData2D>
{
public:
	SpriteRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	virtual void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void Render(const std::shared_ptr<Texture>& s, int x, int y, int sizeX, int sizeY);
private:
	Mesh m_spriteMesh;
};