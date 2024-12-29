#include "pch.h"
#include "SpriteRenderer.hpp"
#include "MeshFactory.h"

using namespace Windows::Foundation;

SpriteRenderer::SpriteRenderer(ID3D11DeviceContext3* context, std::shared_ptr<Texture> emptyTexture)
	: emptyTexture(emptyTexture)
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
}

void SpriteRenderer::BeginRendering(ID3D11DeviceContext3* context, D3D11_VIEWPORT viewport)
{

	m_spriteBatch->SetViewport(viewport);

	// save pipeline state
	context->OMGetDepthStencilState(&state.pDepthStencilState, &state.stencilRef);
	context->OMGetBlendState(&state.pBlendState, state.blendFactor, &state.sampleMask);
	context->RSGetState(&state.pRasterizerState);

	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred);
}

void SpriteRenderer::EndRendering(ID3D11DeviceContext3* context)
{
	m_spriteBatch->End();

	context->OMSetDepthStencilState(state.pDepthStencilState, state.stencilRef);
	context->OMSetBlendState(state.pBlendState, state.blendFactor, state.sampleMask);
	context->RSSetState(state.pRasterizerState);

	if (state.pDepthStencilState) state.pDepthStencilState->Release();
	if (state.pBlendState) state.pBlendState->Release();
	if (state.pRasterizerState) state.pRasterizerState->Release();
}


void SpriteRenderer::Render(const std::shared_ptr<Texture>& texture, int x, int y, int sizeX, int sizeY)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + sizeX;
	rect.bottom = y + sizeY;

	this->m_spriteBatch->Draw(texture->shaderResourceView.Get(), rect);
}

void SpriteRenderer::Render(DirectX::XMFLOAT4 color, int x, int y, int sizeX, int sizeY)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + sizeX;
	rect.bottom = y + sizeY;

	
	this->m_spriteBatch->Draw(emptyTexture->shaderResourceView.Get(), rect, DirectX::XMLoadFloat4(&color));
}
