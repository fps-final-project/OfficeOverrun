#include "pch.h"
#include "CppUnitTest.h"
#include "SubgraphMatcher.h"
#include "RoomLabel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WorldGenerator;

namespace FirstPersonShooter_WorldGeneration_Test
{
    TEST_CLASS(SubgraphMatcherTest)
    {
        Node<void*> MakeNode(int label)
        {
            Node<void*> node(nullptr);
            node.label = label;
            return node;
        }

        std::vector<Node<void*>> MakeNodes(std::vector<int> labels)
        {
            std::vector<Node<void*>> nodes;
            for (int l : labels)
            {
                nodes.push_back(MakeNode(l));
            }
            return nodes;
        }

    public:
        TEST_METHOD(MatchSubgraph_MatchesGraphs)
        {
            // Pattern graph
            Graph<void*> G1 = Graph<void*>(3);
            G1.AddNodes(MakeNodes({ RoomLabel::Normal, RoomLabel::Stairs, RoomLabel::Normal }));
            G1.AddUndirectedEdge(0, 1);
            G1.AddUndirectedEdge(0, 2);
            G1.AddUndirectedEdge(1, 2);

            // Main graph
            Graph<void*> G2 = Graph<void*>(5);
            G2.AddNodes(MakeNodes({ RoomLabel::Normal, RoomLabel::Normal, RoomLabel::Stairs, RoomLabel::Normal, RoomLabel::Normal }));
            G2.AddUndirectedEdge(0, 1);
            G2.AddUndirectedEdge(1, 2);
            G2.AddUndirectedEdge(1, 3);
            G2.AddUndirectedEdge(1, 4);
            G2.AddUndirectedEdge(2, 3);
            G2.AddUndirectedEdge(3, 4);

            SubgraphMatcher::MatchSubgraph(G1, G2);
        }
    };
}