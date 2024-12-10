#pragma once

#include "Common\DeviceResources.h"
#include <memory>

struct __declspec(dllexport) MenuResponse
{
	bool exit;
	bool changeSeedAndRestart;
	int seed;

	MenuResponse() : exit(false), changeSeedAndRestart(false), seed(-1) {}
};

struct ImFont;

class __declspec(dllexport) Menu
{
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	ImFont* buttonFont;
	ImFont* labelFont;
	int currentSeed;
public:
	Menu(std::shared_ptr<DX::DeviceResources> deviceResources, int currentSeed);
	~Menu();
	MenuResponse RenderAndGetResponse(Windows::Foundation::Size screenSize, bool paused);
};