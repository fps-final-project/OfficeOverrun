#include "pch.h"
#include "GraphTestUtils.h"

using namespace WorldGenerator;

Node<void*> FirstPersonShooter_WorldGeneration_Test::GraphTestUtils::MakeNode(int label)
{
    Node<void*> node(nullptr);
    node.label = label;
    return node;
}

std::vector<Node<void*>> FirstPersonShooter_WorldGeneration_Test::GraphTestUtils::MakeNodes(const std::vector<int>& labels)
{
    std::vector<Node<void*>> nodes;
    for (int l : labels)
    {
        nodes.push_back(MakeNode(l));
    }
    return nodes;
}
