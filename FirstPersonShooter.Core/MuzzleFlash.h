#pragma once

#include "Drawable.hpp"

class SpriteRenderer;

class __declspec(dllexport) MuzzleFlash {
	const int DEFAULT_WIDTH = 1024;
	const int DEFAULT_HEIGHT = 768;

	float timeElapsed;
	float timePerFrame;
	int currFrame;
	int frameCount;
	bool isPlaying;
	int sizeX, sizeY, posX, posY;
public:
	MuzzleFlash();
	void SetPosition(int middleX, int middleY);
	void PlayAnimation();
	bool IsPlaying() const { return isPlaying; }
	void Update(float dt);
	void Render(std::shared_ptr<SpriteRenderer> spriteRenderer, int screenWidth, int screenHeight);

};