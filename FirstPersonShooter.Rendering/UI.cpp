#include "pch.h"
#include "UI.hpp"

void UI::RenderCrosshair(Windows::Foundation::Size screenSize, std::shared_ptr<SpriteRenderer> spriteRenderer, std::shared_ptr<Texture> crosshairTexture)
{
	const int size = 100;
	spriteRenderer->Render(crosshairTexture,
		(screenSize.Width - size) / 2,
		(screenSize.Height - size) / 2,
		size,
		size);
}

void UI::RenderDamageIndicator(Windows::Foundation::Size screenSize, std::shared_ptr<SpriteRenderer> spriteRenderer, std::shared_ptr<Texture> texture)
{
	spriteRenderer->Render(texture,
		0,
		0,
		screenSize.Width,
		screenSize.Height);
}

void UI::RenderHealth(Windows::Foundation::Size screenSize, std::shared_ptr<SpriteRenderer> spriteRenderer,
	int maxHealth, int remainingHealth)
{
	float healthPrecentage = (float)remainingHealth / maxHealth;
	int barWidth = screenSize.Width / 5;
	const int barHeight = 30;

	const int bottomOffset = 20;
	const int rightOffset = 20;

	spriteRenderer->Render({1.f, 0.f, 0.f, 1.f},
		screenSize.Width - barWidth * healthPrecentage - rightOffset,
		screenSize.Height - barHeight - bottomOffset,
		barWidth * healthPrecentage,
		barHeight);
}

void UI::RenderBulletCapacity(Windows::Foundation::Size screenSize, std::shared_ptr<SpriteRenderer> spriteRenderer, 
	std::shared_ptr<FirstPersonShooter::SampleFpsTextRenderer> fpsTextRenderer, std::shared_ptr<Texture> bulletTexture, 
	int numBullets, int maxBullets)
{
	const int bottomOffset = 70;
	const int rightOffset = 20;

	const int textMaxHeight = 30;
	const int textMaxWidth = 200;

	const int iconSize = 50;

	spriteRenderer->Render(bulletTexture,
		screenSize.Width - iconSize - rightOffset,
		screenSize.Height - iconSize - bottomOffset,
		iconSize,
		iconSize);

	fpsTextRenderer->Render(std::to_string(numBullets) + "/" + std::to_string(maxBullets),
		rightOffset + iconSize + textMaxWidth,
		bottomOffset + textMaxHeight,
		textMaxWidth,
		textMaxHeight);

}
