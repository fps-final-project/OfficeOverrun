#include "pch.h"
#include "CppUnitTest.h"
#include "GraphProduction.h"
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
            G2.AddUndirectedEdge(2, 3);
            G2.AddUndirectedEdge(3, 4);
            return G2;
        }
	public:
        TEST_METHOD(Apply_AppliesProduction_Relabel)
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
            G_R.AddUndirectedEdge(1, 2);

            GraphProduction P(0, G_L, G_R);

            // Main graph
            auto G = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default, RoomLabel::Default });
            std::vector<int> mapping = { 1, 2, 3 };

            P.Apply(G, mapping);

            Assert::AreEqual((int)RoomLabel::Boss, G[1].label);
            Assert::AreEqual((int)RoomLabel::Stairs, G[2].label);
            Assert::AreEqual((int)RoomLabel::Treasure, G[3].label);

            Assert::IsTrue(G.HasEdge(1, 2));
            Assert::IsTrue(G.HasEdge(1, 3));
            Assert::IsTrue(G.HasEdge(2, 3));
        }

		TEST_METHOD(Apply_AppliesProduction_EdgeRemoval)
		{
            Graph<void*> G_L = Graph<void*>(3);
            G_L.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default }));
            G_L.AddUndirectedEdge(0, 1);
            G_L.AddUndirectedEdge(1, 2);
            G_L.AddUndirectedEdge(0, 2);

            Graph<void*> G_R = Graph<void*>(3);
            G_R.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default }));
            G_R.AddUndirectedEdge(0, 1);
            G_R.AddUndirectedEdge(0, 2);

            GraphProduction P(0, G_L, G_R);

            // Main graph
            auto G = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default, RoomLabel::Default });
            std::vector<int> mapping = { 1, 2, 3 };
            
            P.Apply(G, mapping);

            Assert::AreEqual((int)RoomLabel::Default, G[1].label);
            Assert::AreEqual((int)RoomLabel::Stairs, G[2].label);
            Assert::AreEqual((int)RoomLabel::Default, G[3].label);

            Assert::IsTrue(G.HasEdge(1, 2));
            Assert::IsTrue(G.HasEdge(1, 3));
            Assert::IsFalse(G.HasEdge(2, 3));
		}

        TEST_METHOD(Apply_AppliesProduction_DefaultWildCardRelabel)
        {
            Graph<void*> G_L = Graph<void*>(2);
            G_L.AddNodes(GraphTestUtils::MakeNodes({ -1, RoomLabel::Default }));
            G_L.AddUndirectedEdge(0, 1);

            Graph<void*> G_R = Graph<void*>(2);
            G_R.AddNodes(GraphTestUtils::MakeNodes({ -1, RoomLabel::Treasure}));
            G_R.AddUndirectedEdge(0, 1);

            GraphProduction P(0, G_L, G_R);

            // Main graph
            auto G = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default, RoomLabel::Default });
            std::vector<int> mapping = { 1, 0};

            P.Apply(G, mapping);

            Assert::AreEqual((int)RoomLabel::Treasure, G[0].label);
            Assert::AreEqual((int)RoomLabel::Default, G[1].label);

            Assert::IsTrue(G.HasEdge(0, 1));
        }

        TEST_METHOD(Apply_AppliesProduction_StairsWildCardRelabel)
        {
            Graph<void*> G_L = Graph<void*>(2);
            G_L.AddNodes(GraphTestUtils::MakeNodes({ -1, RoomLabel::Default }));
            G_L.AddUndirectedEdge(0, 1);

            Graph<void*> G_R = Graph<void*>(2);
            G_R.AddNodes(GraphTestUtils::MakeNodes({ -1, RoomLabel::Treasure }));
            G_R.AddUndirectedEdge(0, 1);

            GraphProduction P(0, G_L, G_R);

            // Main graph
            auto G = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default, RoomLabel::Default });
            std::vector<int> mapping = { 2, 3 };

            P.Apply(G, mapping);

            Assert::AreEqual((int)RoomLabel::Treasure, G[3].label);
            Assert::AreEqual((int)RoomLabel::Stairs, G[2].label);

            Assert::IsTrue(G.HasEdge(2, 3));
        }
	};
}