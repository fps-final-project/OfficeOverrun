#pragma once

#include "Drawable.hpp"

class SpriteRenderer;

class MuzzleFlash {
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
	void Update(float dt);
	void Render(std::shared_ptr<SpriteRenderer> spriteRenderer);

};