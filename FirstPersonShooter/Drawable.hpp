#pragma once
#include "RenderMaster.hpp"

class Drawable
{
public:
	virtual ~Drawable() {}
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster) = 0;
};

