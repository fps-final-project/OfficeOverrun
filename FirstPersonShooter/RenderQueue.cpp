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
	renderMaster->getModelRenderer()->ClearStencilBuffer();
	GUID ret = GUID();
	GUID mapping[256] = { GUID{} };
	UINT8 i = 1;
	while (!m_queue.empty())
	{
		mapping[i] = m_queue.top().drawable->id;
		renderMaster->getModelRenderer()->SetSencilBufferRefernceValue(i++);
		m_queue.top().drawable->Render(renderMaster);
		m_queue.pop();
		if (i == 0)
		{
			UINT8 stencilValue = renderMaster->getModelRenderer()->GetStencilBufferValue();
			if (stencilValue > 0)
			{
				ret = mapping[stencilValue];
			}
			i = 1;
		}
	}
	
	UINT8 stencilValue = renderMaster->getModelRenderer()->GetStencilBufferValue();
	if (stencilValue > 0)
	{
		ret = mapping[stencilValue];
	}
	return ret;
}
