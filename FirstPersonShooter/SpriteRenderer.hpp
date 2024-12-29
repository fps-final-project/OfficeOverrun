#pragma once
#include <SpriteBatch.h>
#include <memory>
#include "Texture.h"

struct PipelineState
{
	ID3D11DepthStencilState* pDepthStencilState;
	UINT stencilRef;

	ID3D11BlendState* pBlendState;
	FLOAT blendFactor[4];
	UINT sampleMask;

	ID3D11RasterizerState* pRasterizerState;
};


// wrapper for the SpriteBatch class 
class __declspec(dllexport) SpriteRenderer
{
public:
	SpriteRenderer(ID3D11DeviceContext3* context, std::shared_ptr<Texture> emptyTexture);
	void BeginRendering(ID3D11DeviceContext3* context, D3D11_VIEWPORT viewport);
	void EndRendering(ID3D11DeviceContext3* context);
	void Render(const std::shared_ptr<Texture>& s, int x, int y, int sizeX, int sizeY);
	void Render(DirectX::XMFLOAT4 color, int x, int y, int sizeX, int sizeY);
private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::shared_ptr<Texture> emptyTexture;
	PipelineState state;
};