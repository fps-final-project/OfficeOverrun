#pragma once

#include <list>
#include "PathNodeData.h"

struct Path
{
	std::list<PathNodeData> path;
	int currentNode;
	bool playerVisible;
};