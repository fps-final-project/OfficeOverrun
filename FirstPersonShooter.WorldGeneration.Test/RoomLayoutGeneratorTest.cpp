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

        void PopulateRooms(RoomLayoutGenerator& generator)
        {
            // Add some rooms
            std::vector<GeneratedRoom> rooms;
            rooms.push_back(MakeGeneratedRoom(0, 0, 0, 4, 4));

            generator.layout.rooms = rooms;
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

        }
    };
}