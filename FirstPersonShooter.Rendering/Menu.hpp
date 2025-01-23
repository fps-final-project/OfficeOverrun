#pragma once

#include "Common\DeviceResources.h"
#include <memory>

struct __declspec(dllexport) MenuResponse
{
	bool exit;
	bool changeSeedAndRestart;
	bool volumeChanged;
	float volume;
	int seed;

	MenuResponse() : exit(false), changeSeedAndRestart(false), seed(-1) {}
};

struct ImFont;
struct ImVec2;

class __declspec(dllexport) Menu
{
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	ImFont* buttonFont, *labelFont, *titleFont;
	int currentSeed;
	float currentVolume;
public:
	Menu(std::shared_ptr<DX::DeviceResources> deviceResources, int currentSeed, float currentVolume);
	~Menu();
	void StartNewFrame();
	void FinishFrame();
	MenuResponse RenderDefaultAndGetResponse(Windows::Foundation::Size screenSize);
	MenuResponse RenderGameWonAndGetResponse(Windows::Foundation::Size screenSize);
	MenuResponse RenderGameLostAndGetResponse(Windows::Foundation::Size screenSize);
	void RenderCommonComponents(MenuResponse& response, int elementWidth, int elementHeight, ImVec2 windowSize);
};