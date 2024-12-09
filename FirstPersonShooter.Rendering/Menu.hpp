#pragma once

#include "Common\DeviceResources.h"
#include <memory>

class __declspec(dllexport) Menu
{
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
public:
	Menu(std::shared_ptr<DX::DeviceResources> deviceResources);
	~Menu();
	void Render(Windows::Foundation::Size screenSize);
};