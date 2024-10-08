#pragma once
#include <memory>
#include <queue>
#include "RendererType.hpp"
#include "Drawable.hpp"

struct RenderData
{
	RendererType type;
	Drawable* drawable;
	RenderData(RendererType type, Drawable* d) : type(type), drawable(d) {}
};

class RenderDataComparer
{
public:
	bool operator()(const RenderData& rd1, const RenderData& rd2)
	{
		return rd1.type < rd2.type;
	}
};

class RenderQueue
{
public:
	RenderQueue();
	void push(const RenderData& data);
	GUID drawAllAndClear(std::shared_ptr<RenderMaster> renderMaster);
private:
	// The idea is to sort the draw calls
	// The most obvious part is sorting by Renderer Type - such that we do not need
	// to load the same shader and/or its parameters each draw call
	// but there is room for improvement ex. sorting by model/mesh used or textures used, so once
	// they're loaded they can be reused
	// We probably will not need it though, still handy to have an idea what could be implemented here
	std::priority_queue<RenderData, std::vector<RenderData>, RenderDataComparer> m_queue;
};