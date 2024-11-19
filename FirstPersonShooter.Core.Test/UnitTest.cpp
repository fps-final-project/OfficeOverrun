#include "pch.h"
#include "CppUnitTest.h"

#include "Player.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FirstPersonShooter_Core_Test
{
    TEST_CLASS(UnitTest1)
    {
    public:
        TEST_METHOD(TestMethod1)
        {
            Player player;
            Assert::AreEqual((int)player.getPostition().x, 0);
        }
    };
}