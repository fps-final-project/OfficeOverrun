#include "pch.h"
#include "CppUnitTest.h"
#include "RoomLink.h"
#include "TestUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WorldGenerator;

namespace FirstPersonShooter_WorldGeneration_Test
{

    TEST_CLASS(RoomLinkTest)
    {

    public:
        TEST_METHOD(MakeRoomLink_ValidYZLinkOrientation)
        {
            Vector3 pos(4, 8, 2);
            Vector3 size(0, 10, 1);

            auto link = RoomLink::MakeRoomLink(pos, size);

            Assert::IsTrue(link.orientation == Orientation::YZ);
        }

        TEST_METHOD(MakeRoomLink_ValidYZLinkPos)
        {
            Vector3 pos(4, 8, 2);
            Vector3 size(0, 10, 1);

            auto link = RoomLink::MakeRoomLink(pos, size);

            Assert::IsTrue(link.pos.y >= 9);
            Assert::IsTrue(link.pos.y <= 17);
            Assert::AreEqual(2, link.pos.z);
        }

        TEST_METHOD(MakeRoomLink_ValidXZLinkOrientation)
        {
            Vector3 pos(4, 8, 2);
            Vector3 size(10, 0, 1);

            auto link = RoomLink::MakeRoomLink(pos, size);

            Assert::IsTrue(link.orientation == Orientation::XZ);
        }

        TEST_METHOD(MakeRoomLink_ValidXZLinkPos)
        {
            Vector3 pos(4, 8, 2);
            Vector3 size(10, 0, 1);

            auto link = RoomLink::MakeRoomLink(pos, size);

            Assert::IsTrue(link.pos.x >= 5);
            Assert::IsTrue(link.pos.x <= 13);
            Assert::AreEqual(2, link.pos.z);
        }

        TEST_METHOD(MakeRoomLink_ValidXYXLinkOrientation)
        {
            Vector3 pos(4, 8, 2);
            Vector3 size(10, 5, 0);

            auto link = RoomLink::MakeRoomLink(pos, size);

            Assert::IsTrue(link.orientation == Orientation::XYX);
        }

        TEST_METHOD(MakeRoomLink_ValidXYXLinkPos)
        {
            Vector3 pos(4, 8, 2);
            Vector3 size(10, 5, 0);

            auto link = RoomLink::MakeRoomLink(pos, size);

            Assert::IsTrue(link.pos.x >= 5);
            Assert::IsTrue(link.pos.x <= 8);
            Assert::IsTrue(link.pos.y >= 9);
            Assert::IsTrue(link.pos.y <= 10);
            Assert::AreEqual(1, link.pos.z);
        }

        TEST_METHOD(MakeRoomLink_ValidXYYLinkOrientation)
        {
            Vector3 pos(4, 8, 2);
            Vector3 size(5, 10, 0);

            auto link = RoomLink::MakeRoomLink(pos, size);

            Assert::IsTrue(link.orientation == Orientation::XYY);
        }

        TEST_METHOD(MakeRoomLink_ValidXYYLinkPos)
        {
            Vector3 pos(4, 8, 1);
            Vector3 size(6, 10, 0);

            auto link = RoomLink::MakeRoomLink(pos, size);

            Assert::IsTrue(link.pos.x >= 5);
            Assert::IsTrue(link.pos.x <= 7);
            Assert::IsTrue(link.pos.y >= 9);
            Assert::IsTrue(link.pos.y <= 12);
            Assert::AreEqual(0, link.pos.z);
        }
    };
}