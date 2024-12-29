#pragma once

#include "SpriteRenderer.hpp"
#include "Content\SampleFpsTextRenderer.h"
#include "Texture.h"

class __declspec(dllexport) UI
{
public:
	static void RenderCrosshair(
		Windows::Foundation::Size screenSize,
		std::shared_ptr<SpriteRenderer> spriteRenderer,
		std::shared_ptr<Texture> crosshairTexture);

	static void RenderDamageIndicator(
		Windows::Foundation::Size screenSize,
		std::shared_ptr<SpriteRenderer> spriteRenderer,
		std::shared_ptr<Texture> texture);

	static void RenderHealth(
		Windows::Foundation::Size screenSize,
		std::shared_ptr<SpriteRenderer> spriteRenderer,
		int maxHealth, int remainingHealth);

	static void RenderBulletCapacity(
		Windows::Foundation::Size screenSize,
		std::shared_ptr<SpriteRenderer> spriteRenderer,
		std::shared_ptr<FirstPersonShooter::SampleFpsTextRenderer> fpsTextRenderer,
		std::shared_ptr<Texture> bulletTexture,
		int numBullets, int maxBullets);
};