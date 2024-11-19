#pragma once
#include "RenderMaster.hpp"

class __declspec(dllexport) Drawable
{
public:
	GUID id;
	Drawable() { CoCreateGuid(&id); }
	virtual ~Drawable(){}
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) = 0;
};
