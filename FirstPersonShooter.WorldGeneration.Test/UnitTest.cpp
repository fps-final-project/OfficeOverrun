#include "pch.h"
#include "CppUnitTest.h"

#include "Vector3.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FirstPersonShooter_WorldGeneration_Test
{
    TEST_CLASS(UnitTest1)
    {
    public:
        TEST_METHOD(TestMethod1)
        {
            WorldGenerator::Vector3 v(1, 1, 1);

            Assert::AreEqual(v.x, 1);
        }
    };
}