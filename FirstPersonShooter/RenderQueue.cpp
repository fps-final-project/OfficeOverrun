#include "pch.h"
#include "RenderQueue.hpp"

RenderQueue::RenderQueue()
{
}

void RenderQueue::push(const RenderData& data)
{
	m_queue.push(data);
}

void RenderQueue::drawAllAndClear(std::shared_ptr<RenderMaster> renderMaster)
{	
	while (!m_queue.empty())
	{
		m_queue.top().drawable->Render(renderMaster);
		m_queue.pop();
	}
}
