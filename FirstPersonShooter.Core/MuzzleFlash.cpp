#include "pch.h"
#include "MuzzleFlash.h"
#include "SpriteRenderer.hpp"
#include "ResourceManager.h"

MuzzleFlash::MuzzleFlash() 
	: timeElapsed(0), timePerFrame(0.003), frameCount(21), 
	currFrame(1), isPlaying(false), sizeX(128), sizeY(128),
	posX(-1000), posY(-1000)
{
}

void MuzzleFlash::SetPosition(int middleX, int middleY)
{
	this->posX = middleX - sizeX / 2;
	this->posY = middleY - sizeY / 2;
}

void MuzzleFlash::PlayAnimation()
{
	timeElapsed = 0;
	currFrame = (rand() % 3) + 1;
	isPlaying = true;
}

void MuzzleFlash::Update(float dt)
{
	if (isPlaying)
	{
		timeElapsed += dt;
		while (timeElapsed > timePerFrame * currFrame)
			currFrame++;

		if (currFrame > frameCount)
		{
			isPlaying = false;
		}
	}
}

void MuzzleFlash::Render(std::shared_ptr<SpriteRenderer> spriteRenderer, int screenWidth, int screenHeight)
{
	if (isPlaying)
	{
		spriteRenderer->Render(ResourceManager::Instance().getTexture("muzzle_" + std::to_string(currFrame)),
			posX * screenWidth / DEFAULT_WIDTH, 
			posY * screenHeight / DEFAULT_HEIGHT,
			sizeX, sizeY);

	}
}
