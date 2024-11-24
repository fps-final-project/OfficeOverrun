#include "pch.h"
#include "CppUnitTest.h"
#include "RoomLayoutGenerator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WorldGenerator;

namespace FirstPersonShooter_WorldGeneration_Test
{
    TEST_CLASS(RoomLayoutGeneratorTest)
    {
        RoomLayoutConfig _config;
        
        GeneratedRoom MakeGeneratedRoom(int posX, int posY, int posZ, int sizeX, int sizeY)
        {
            GeneratedRoom room;
            room.pos = Vector3(posX, posY, posZ);
            room.size = Vector3(sizeX, sizeY, 1);

            return room;
        }

        RoomLayout MakeLayoutWithRooms()
        {
            RoomLayout layout(_config.mapSize);
            // Add some rooms
            std::vector<GeneratedRoom> rooms;
            rooms.push_back(MakeGeneratedRoom(4, 0, 0, 4, 4));
            rooms.push_back(MakeGeneratedRoom(0, 4, 0, 12, 12));
            rooms.push_back(MakeGeneratedRoom(4, 8, 1, 9, 12));

            layout.rooms = rooms;

            return layout;
        }

        bool VectorsEqual(Vector3 v1, Vector3 v2)
        {
            return v1.x == v2.x
                && v1.y == v2.y
                && v1.z == v2.z;
        }

        bool RoomsEqual(GeneratedRoom r1, GeneratedRoom r2)
        {
            return VectorsEqual(r1.pos, r2.pos) && VectorsEqual(r1.size, r2.size);
        }

        void PopulateAdGraphEdges(Graph<GeneratedRoom> &graph)
        {
            graph.AddUndirectedEdge(0, 1);
            graph.AddUndirectedEdge(1, 2);
        }
        
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            _config = RoomLayoutConfig();
            _config.mapSize = Vector3(30, 30, 3);
            _config.roomDensity = 0.6;
            _config.pathLengthCoeff = 0.6;
            _config.edgeDensityCoeff = 0.3;
        }

        TEST_METHOD(GenerateAdGraph_GraphContainsAllRooms)
        {
            RoomLayoutGenerator roomLayoutGenerator(_config);
            RoomLayout layout = MakeLayoutWithRooms();

            auto adGraph = roomLayoutGenerator.GenerateAdGraph(layout);

            Assert::AreEqual(layout.rooms.size(), adGraph.nodes.size());
        }

        TEST_METHOD(GenerateAdGraph_GraphRoomsInTheSameOrder)
        {
            RoomLayoutGenerator roomLayoutGenerator(_config);
            RoomLayout layout = MakeLayoutWithRooms();

            auto adGraph = roomLayoutGenerator.GenerateAdGraph(layout);

            bool validOrder = true;

            for (int i = 0; i < layout.rooms.size(); i++)
            {
                GeneratedRoom room = layout.rooms[i];
                GeneratedRoom graphRoom = *adGraph.nodes[i].value;

                validOrder &= RoomsEqual(room, graphRoom);
            }
            
            Assert::IsTrue(validOrder);
        }

        TEST_METHOD(GenerateAdGraph_GraphContainsAllValidLinks)
        {
            RoomLayoutGenerator roomLayoutGenerator(_config);
            RoomLayout layout = MakeLayoutWithRooms();

            auto adGraph = roomLayoutGenerator.GenerateAdGraph(layout);

            Assert::IsTrue(adGraph.HasEdge(0, 1));
            Assert::IsTrue(adGraph.HasEdge(1, 0));
            Assert::IsTrue(adGraph.HasEdge(1, 2));
            Assert::IsTrue(adGraph.HasEdge(2, 1));
            Assert::IsFalse(adGraph.HasEdge(0, 2));
            Assert::IsFalse(adGraph.HasEdge(2, 0));
        }

        TEST_METHOD(GenerateLayoutFromAdGraph_GeneratesLinks)
        {
            RoomLayoutGenerator roomLayoutGenerator(_config);
            RoomLayout layout = MakeLayoutWithRooms();
            Graph<GeneratedRoom> graph(layout.rooms);
            PopulateAdGraphEdges(graph);

            roomLayoutGenerator.GenerateRoomLinks(graph);

            int linksCount = 0;
            for (auto node : graph.nodes)
            {
                linksCount += (*node.value).links.size();
            }

            Assert::AreEqual(4, linksCount);
        }

        TEST_METHOD(GenerateLayoutFromAdGraph_GeneratesValidLinks)
        {
            RoomLayoutGenerator roomLayoutGenerator(_config);
            RoomLayout layout = MakeLayoutWithRooms();
            Graph<GeneratedRoom> graph(layout.rooms);
            PopulateAdGraphEdges(graph);

            roomLayoutGenerator.GenerateRoomLinks(graph);

            bool validLinks = true;
            for (int i = 0; i < graph.Size(); i++)
            {
                auto node = graph[i];
                for (auto link : (*node.value).links)
                {
                    validLinks &= graph.HasEdge(i, link.linkedRoomIdx);
                }
            }

            Assert::IsTrue(validLinks);
        }

        TEST_METHOD(GenerateLayoutFromAdGraph_LinksSymmetric)
        {
            RoomLayoutGenerator roomLayoutGenerator(_config);
            RoomLayout layout = MakeLayoutWithRooms();
            Graph<GeneratedRoom> graph(layout.rooms);
            PopulateAdGraphEdges(graph);

            roomLayoutGenerator.GenerateRoomLinks(graph);

            bool validLinks = true;
            for (int i = 0; i < graph.Size(); i++)
            {
                auto node = graph[i];
                for (auto link : (*node.value).links)
                {
                    auto linkedNode = *graph[link.linkedRoomIdx].value;
                    auto linkedTo = [&](RoomLink link) { return link.linkedRoomIdx == i; };
                    auto found = std::find_if(linkedNode.links.begin(), linkedNode.links.end(), linkedTo);

                    validLinks &= found != linkedNode.links.end();
                    validLinks &= link.orientation ==  (*found).orientation;
                    validLinks &= VectorsEqual(link.pos, (*found).pos);
                }
            }

            Assert::IsTrue(validLinks);
        }

        TEST_METHOD(GenerateLayoutFromAdGraph_GeneratesRoomsInTheSameOrder)
        {
            RoomLayoutGenerator roomLayoutGenerator(_config);
            RoomLayout layout = MakeLayoutWithRooms();
            Graph<GeneratedRoom> graph(layout.rooms);
            PopulateAdGraphEdges(graph);

            auto newLayout = roomLayoutGenerator.GenerateLayoutFromAdGraph(graph);

            bool validRooms = true;
            for (int i = 0; i < graph.Size(); i++)
            {
                validRooms &= RoomsEqual(*graph[i].value, newLayout.rooms[i]);
            }

            Assert::IsTrue(validRooms);
        }
    };
}