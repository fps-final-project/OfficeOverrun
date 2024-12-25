#include "pch.h"
#include "CppUnitTest.h"
#include "GraphGrammar.h"
#include "GraphTestUtils.h"
#include "RoomLabel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WorldGenerator;

namespace FirstPersonShooter_WorldGeneration_Test
{
    TEST_CLASS(GraphProductionTest)
    {
        Graph<void*> MakeMainGraph(std::vector<int> labels)
        {
            Graph<void*> G2 = Graph<void*>(5);
            G2.AddNodes(GraphTestUtils::MakeNodes(labels));
            G2.AddUndirectedEdge(0, 1);
            G2.AddUndirectedEdge(1, 2);
            G2.AddUndirectedEdge(1, 3);
            G2.AddUndirectedEdge(1, 4);
            G2.AddUndirectedEdge(3, 4);
            G2.AddUndirectedEdge(2, 4);
            return G2;
        }

        GraphProduction<void*> MakeTriangleStairsProduction()
        {
            Graph<void*> G_L = Graph<void*>(3);
            G_L.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default }));
            G_L.AddUndirectedEdge(0, 1);
            G_L.AddUndirectedEdge(1, 2);
            G_L.AddUndirectedEdge(0, 2);

            Graph<void*> G_R = Graph<void*>(3);
            G_R.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Boss, RoomLabel::Stairs, RoomLabel::Treasure }));
            G_R.AddUndirectedEdge(0, 1);
            G_R.AddUndirectedEdge(0, 2);

            return GraphProduction(10, G_L, G_R);
        }

        GraphProduction<void*> MakeDetachStairsProduction()
        {
            Graph<void*> G_L = Graph<void*>(3);
            G_L.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Boss, RoomLabel::Stairs, RoomLabel::Default }));
            G_L.AddUndirectedEdge(0, 1);
            G_L.AddUndirectedEdge(1, 2);

            Graph<void*> G_R = Graph<void*>(3);
            G_R.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Boss, RoomLabel::Stairs, RoomLabel::Default }));
            G_R.AddUndirectedEdge(0, 1);

            return GraphProduction(0, G_L, G_R);
        }
    public:
        TEST_METHOD(Apply_AppliesGrammar_LeaveOnlyBossAdjacentToStairs)
        {
            // Main graph
            auto G = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs });

            // Productions
            std::vector<GraphProduction<void*>> prods = { MakeTriangleStairsProduction(), MakeDetachStairsProduction() };

            GraphGrammar<void*> grammar(prods);

            grammar.Apply(G);

            auto ad_to_stairs = G.GetNeighbours(4);
            Assert::AreEqual(1, (int)ad_to_stairs.size());
            Assert::AreEqual((int)RoomLabel::Boss, G[ad_to_stairs[0]].label);
        }

        TEST_METHOD(Apply_AppliesGrammar_NoAvailableProductions)
        {
            // Main graph
            auto G = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs });

            // Productions
            std::vector<GraphProduction<void*>> prods = { MakeDetachStairsProduction() };

            GraphGrammar<void*> grammar(prods);

            grammar.Apply(G);

            auto ad_to_stairs = G.GetNeighbours(4);
            Assert::AreEqual(3, (int)ad_to_stairs.size());
         
            bool isDefaultOrStairs = true;
            for (auto node : G.nodes)
                isDefaultOrStairs &= (node.label == RoomLabel::Default || node.label == RoomLabel::Stairs);
            Assert::IsTrue(isDefaultOrStairs);
        }

        TEST_METHOD(Apply_AppliesGrammar_AddsTreasureRooms)
        {
            // Main graph
            auto G = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs });

            // Productions
            std::vector<GraphProduction<void*>> prods = { MakeTriangleStairsProduction() };

            GraphGrammar<void*> grammar(prods);

            grammar.Apply(G);

            auto ad_to_stairs = G.GetNeighbours(4);
            Assert::AreEqual(2, (int)ad_to_stairs.size());

            bool isTreasureRoom = false;
            for (auto node : G.nodes)
                isTreasureRoom |= node.label == RoomLabel::Treasure;
            Assert::IsTrue(isTreasureRoom);
        }
    };
}