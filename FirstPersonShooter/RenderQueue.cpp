#include "pch.h"
#include "RenderQueue.hpp"

RenderQueue::RenderQueue()
{
}

void RenderQueue::Push(const RenderData& data)
{
	m_queue.push(data);
}

GUID RenderQueue::DrawAllAndClear(std::shared_ptr<RenderMaster> renderMaster)
{	
	renderMaster->ClearStencilBuffer();
	GUID ret = GUID();
	GUID mapping[256] = { GUID{} };
	UINT8 i = 1;

	while (!m_queue.empty())
	{
		mapping[i] = m_queue.top().drawable->id;
		renderMaster->SetRenderer(m_queue.top().type);
		renderMaster->SetStencilBufferReferenceValue(i++);
		m_queue.top().drawable->Render(renderMaster);
		m_queue.pop();
		if (i == 0)
		{
			UINT8 stencilValue = renderMaster->GetCurrentStencilValue();
			if (stencilValue > 0)
			{
				ret = mapping[stencilValue];
			}
			i = 1;
		}
	}
	
	UINT8 stencilValue = renderMaster->GetCurrentStencilValue();
	if (stencilValue > 0)
	{
		ret = mapping[stencilValue];
	}
	return ret;
}
