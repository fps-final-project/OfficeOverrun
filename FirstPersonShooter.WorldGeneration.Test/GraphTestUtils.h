#pragma once
#include "Graph.h"

using namespace WorldGenerator;
namespace FirstPersonShooter_WorldGeneration_Test
{
	class GraphTestUtils
	{
	public:
		static Node<void*> MakeNode(int label);
		static std::vector<Node<void*>> MakeNodes(const std::vector<int> &labels);
	};
}

