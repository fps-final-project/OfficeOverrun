#include "pch.h"
#include "CppUnitTest.h"

#include "RenderQueue.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FirstPersonShooter_Rendering_Test
{
    TEST_CLASS(UnitTest1)
    {
    public:
        TEST_METHOD(TestMethod1)
        {
            RenderQueue queue;
            queue.Push(RenderData(RendererType::MODEL, nullptr));
            Assert::AreEqual(1, 1);
        }
    };
}