#include "pch.h"
#include "CppUnitTest.h"

#include "BinaryRoom.h"
#include "BinaryRoom.cpp"
#include "RngIncludes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WorldGenerator;

namespace FirstPersonShooter_WorldGeneration_Test
{

    TEST_CLASS(BinaryRoomTest)
    {
        Vector3 _mapSize;
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            _mapSize = Vector3(30, 30, 3);
        }
        TEST_METHOD(Split_RoomsOfValidSize)
        {
            RoomLayout layout(_mapSize);
            
            BinaryRoom::MakeRoomsOnLayoutFloor(layout, 0);

            for (auto room : layout.rooms)
            {
                bool validSize = true;
                // valid x
                validSize &= room.size.x >= RoomLayoutConfig::minRoom2DSize
                    && room.size.x <= RoomLayoutConfig::maxRoom2DSize;
                // valid y
                validSize &= room.size.y >= RoomLayoutConfig::minRoom2DSize
                    && room.size.y <= RoomLayoutConfig::maxRoom2DSize;
                // valid z
                validSize &= room.size.z == 1;

                Assert::IsTrue(validSize);
            }
        }
    };
}