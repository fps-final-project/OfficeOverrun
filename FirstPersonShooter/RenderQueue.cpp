#include "pch.h"
#include "RenderQueue.hpp"

RenderQueue::RenderQueue()
{
}

void RenderQueue::push(const RenderData& data)
{
	m_queue.push(data);
}

GUID RenderQueue::drawAllAndClear(std::shared_ptr<RenderMaster> renderMaster)
{	
	GUID ret = GUID();
	while (!m_queue.empty())
	{
		renderMaster->getModelRenderer()->ClearStencilBuffer();
		m_queue.top().drawable->Render(renderMaster);
		UINT8 stencilValue = renderMaster->getModelRenderer()->GetStencilBufferValue();
		if (stencilValue > 0)
		{
			ret = m_queue.top().drawable->id;
		}
		m_queue.pop();
	}
	return ret;
}
