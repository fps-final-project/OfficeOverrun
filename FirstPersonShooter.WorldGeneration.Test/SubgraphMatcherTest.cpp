#include "pch.h"
#include "CppUnitTest.h"
#include "SubgraphMatcher.h"
#include "RoomLabel.h"
#include "GraphTestUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WorldGenerator;

namespace FirstPersonShooter_WorldGeneration_Test
{
    TEST_CLASS(SubgraphMatcherTest)
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

        TEST_METHOD(MatchSubgraph_FindsAllIsomorphism_TriangleWithNoLabels)
        {
            // Pattern graph
            Graph<void*> G1 = Graph<void*>(3);
            G1.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Default }));
            G1.AddUndirectedEdge(0, 1);
            G1.AddUndirectedEdge(0, 2);
            G1.AddUndirectedEdge(1, 2);

            // Main graph
            auto G2 = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Default, RoomLabel::Default, RoomLabel::Default });

            auto mappings = SubgraphMatcher::MatchSubgraph(G1, G2);

            std::vector<std::vector<int>> expectedMappings({ {1,2,3}, {1,4,3} });

            Assert::AreEqual(mappings.size(), expectedMappings.size() * 6); // contains all permutations of expected sets
            Assert::IsTrue(std::find(mappings.begin(), mappings.end(), expectedMappings[0]) != mappings.end());
            Assert::IsTrue(std::find(mappings.begin(), mappings.end(), expectedMappings[1]) != mappings.end());
        }

        TEST_METHOD(MatchSubgraph_FindsAllIsomorphism_TriangleWithOneLabel)
        {
            // Pattern graph
            Graph<void*> G1 = Graph<void*>(3);
            G1.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default }));
            G1.AddUndirectedEdge(0, 1);
            G1.AddUndirectedEdge(0, 2);
            G1.AddUndirectedEdge(1, 2);

            // Main graph
            auto G2 = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default, RoomLabel::Default });

            auto mappings = SubgraphMatcher::MatchSubgraph(G1, G2);

            std::vector<std::vector<int>> expectedMappings({ {1,2,3}, {3,2,1} });

            Assert::IsTrue(std::find(mappings.begin(), mappings.end(), expectedMappings[0]) != mappings.end());
            Assert::IsTrue(std::find(mappings.begin(), mappings.end(), expectedMappings[1]) != mappings.end());
        }

        TEST_METHOD(MatchSubgraph_FindsAllIsomorphism_TriangleWithDifferentLabels)
        {
            // Pattern graph
            Graph<void*> G1 = Graph<void*>(3);
            G1.AddNodes(GraphTestUtils::MakeNodes({ 1, RoomLabel::Stairs, RoomLabel::Default }));
            G1.AddUndirectedEdge(0, 1);
            G1.AddUndirectedEdge(0, 2);
            G1.AddUndirectedEdge(1, 2);

            // Main graph
            auto G2 = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs, 1, RoomLabel::Default });

            auto mappings = SubgraphMatcher::MatchSubgraph(G1, G2);

            std::vector<std::vector<int>> expectedMappings({ {3,2,1} });

            Assert::IsTrue(std::find(mappings.begin(), mappings.end(), expectedMappings[0]) != mappings.end());
        }

        TEST_METHOD(MatchSubgraph_FindsNoIsomorphism_TriangleWithNonmatchingLabels)
        {
            // Pattern graph
            Graph<void*> G1 = Graph<void*>(3);
            G1.AddNodes(GraphTestUtils::MakeNodes({ 4, RoomLabel::Stairs, RoomLabel::Default }));
            G1.AddUndirectedEdge(0, 1);
            G1.AddUndirectedEdge(0, 2);
            G1.AddUndirectedEdge(1, 2);

            // Main graph
            auto G2 = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs, 1, RoomLabel::Default });

            auto mappings = SubgraphMatcher::MatchSubgraph(G1, G2);

            Assert::IsTrue(mappings.empty());
        }

        TEST_METHOD(MatchSubgraph_FindsAllIsomorphism_OneLabelledVertex)
        {
            // Pattern graph
            Graph<void*> G1 = Graph<void*>(3);
            G1.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Stairs }));

            // Main graph
            auto G2 = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default, RoomLabel::Default });

            auto mappings = SubgraphMatcher::MatchSubgraph(G1, G2);

            std::vector<std::vector<int>> expectedMappings({ {2}});

            Assert::AreEqual(expectedMappings.size(), mappings.size());
            Assert::IsTrue(std::find(mappings.begin(), mappings.end(), expectedMappings[0]) != mappings.end());
        }

        TEST_METHOD(MatchSubgraph_FindsAllIsomorphism_OneNonLabelledVertex)
        {
            // Pattern graph
            Graph<void*> G1 = Graph<void*>(3);
            G1.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Default }));

            // Main graph
            auto G2 = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default, RoomLabel::Default });

            auto mappings = SubgraphMatcher::MatchSubgraph(G1, G2);

            std::vector<std::vector<int>> expectedMappings({ {0}, {1}, {3}, {4} });

            Assert::AreEqual(expectedMappings.size(), mappings.size());
            Assert::IsTrue(std::find(mappings.begin(), mappings.end(), expectedMappings[0]) != mappings.end());
            Assert::IsTrue(std::find(mappings.begin(), mappings.end(), expectedMappings[1]) != mappings.end());
            Assert::IsTrue(std::find(mappings.begin(), mappings.end(), expectedMappings[2]) != mappings.end());
            Assert::IsTrue(std::find(mappings.begin(), mappings.end(), expectedMappings[3]) != mappings.end());
        }

        TEST_METHOD(MatchSubgraph_FindsNoIsomorphism_NonExistantLabel)
        {
            // Pattern graph
            Graph<void*> G1 = Graph<void*>(3);
            G1.AddNodes(GraphTestUtils::MakeNodes({ 2321 }));

            // Main graph
            auto G2 = MakeMainGraph({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Stairs, RoomLabel::Default, RoomLabel::Default });

            auto mappings = SubgraphMatcher::MatchSubgraph(G1, G2);

            std::vector<std::vector<int>> expectedMappings({ {0}, {1}, {3}, {4} });

            Assert::IsTrue(mappings.empty());
        }

        TEST_METHOD(MatchSubgraph_FindsAllIsomorphism_FindTheSameGraph)
        {
            // Pattern graph
            Graph<void*> G1 = Graph<void*>(3);
            G1.AddNodes(GraphTestUtils::MakeNodes({ RoomLabel::Default, RoomLabel::Default, RoomLabel::Default }));
            G1.AddUndirectedEdge(0, 1);
            G1.AddUndirectedEdge(0, 2);
            G1.AddUndirectedEdge(1, 2);

            auto mappings = SubgraphMatcher::MatchSubgraph(G1, G1);

            std::vector<std::vector<int>> expectedMappings({ {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}});

            bool findsAll = true;
            for (auto mapping : expectedMappings)
                findsAll &= (std::find(mappings.begin(), mappings.end(), mapping) != mappings.end());

            Assert::IsTrue(findsAll);
        }
    };
}