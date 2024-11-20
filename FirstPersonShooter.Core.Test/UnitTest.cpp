#include "pch.h"
#include "CppUnitTest.h"

#include "Entity.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FirstPersonShooter_Core_Test
{
    TEST_CLASS(UnitTest1)
    {
    public:
        TEST_METHOD(TestMethod1)
        {
            Entity e(std::make_shared<Model>());
            Assert::AreEqual(0, 0);
        }
    };
}