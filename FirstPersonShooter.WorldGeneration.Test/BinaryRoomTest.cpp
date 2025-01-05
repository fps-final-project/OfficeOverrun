#include "pch.h"
#include "CppUnitTest.h"

#include "BinaryRoom.h"

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
            _mapSize = Vector3(40, 30, 3);
        }
        TEST_METHOD(Split_RoomsOfValidSize)
        {
            RoomLayout layout(_mapSize);
            
            BinaryRoom::MakeRoomsOnLayoutFloor(layout, 0);

            bool validSize = true;
            for (auto room : layout.rooms)
            {
                // valid x
                validSize &= room.size.x >= RoomLayoutConfig::MIN_ROOM_2D_SIZE
                    && room.size.x <= RoomLayoutConfig::MAX_ROOM_2D_SIZE;
                // valid y
                validSize &= room.size.y >= RoomLayoutConfig::MIN_ROOM_2D_SIZE
                    && room.size.y <= RoomLayoutConfig::MAX_ROOM_2D_SIZE;
                // valid z
                validSize &= room.size.z == 1;
            }
            Assert::IsTrue(validSize);
        }

        TEST_METHOD(Split_RoomsOfValidFloor)
        {
            RoomLayout layout(_mapSize);
            int floor = 2;

            BinaryRoom::MakeRoomsOnLayoutFloor(layout, floor);

            bool validFloor = true;
            for (auto room : layout.rooms)
            {
                validFloor &= room.pos.z == floor;
            }
            Assert::IsTrue(validFloor);
        }

        TEST_METHOD(Split_RoomsInsideRootRoom)
        {
            RoomLayout layout(_mapSize);

            BinaryRoom::MakeRoomsOnLayoutFloor(layout, 0);

            bool validPos = true;
            for (auto room : layout.rooms)
            {
                // valid x
                validPos &= room.pos.x >= 0
                    && room.size.x <= _mapSize.x - RoomLayoutConfig::MIN_ROOM_2D_SIZE;
                // valid y
                validPos &= room.pos.y >= 0
                    && room.size.y <= _mapSize.y - RoomLayoutConfig::MIN_ROOM_2D_SIZE;
            }
            Assert::IsTrue(validPos);
        }
    };
}