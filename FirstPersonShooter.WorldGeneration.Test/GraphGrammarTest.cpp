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
        TEST_METHOD(Apply_AppliesGrammar_DeletesTrianglesOverStairs)
        {
            // Main graph
            auto G = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs });
            std::vector<int> mapping = { 1, 2, 3 };

            // Productions
            std::vector<GraphProduction<void*>> prods = { MakeTriangleStairsProduction(), MakeDetachStairsProduction() };

            GraphGrammar<void*> grammar(prods);

            grammar.Apply(G);
        }
    };
}