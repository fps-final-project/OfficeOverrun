#include "pch.h"
#include "CppUnitTest.h"
#include "GraphProductionReader.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WorldGenerator;

namespace FirstPersonShooter_WorldGeneration_Test
{
    TEST_CLASS(GraphProductionReaderTest)
    {
    public:
        TEST_METHOD(ReadGraphProduction_ReadsGraph)
        {
            std::string filePath = R"(.\TestFiles\GraphProductionReaderTest\TestCase1.txt)";

            auto production = GraphProductionReader::ReadGraphProduction<void*>(filePath);

            Assert::AreEqual(2, production.Priority());
            // Left side graph
            auto gl = production.L_graph();
            Assert::AreEqual(3, gl.Size());
            Assert::AreEqual(0, gl[0].label);
            Assert::AreEqual(104, gl[1].label);
            Assert::AreEqual(0, gl[2].label);
            Assert::IsTrue(gl.HasEdge(0,1));
            Assert::IsTrue(gl.HasEdge(1,0));
            Assert::IsTrue(gl.HasEdge(1,2));
            Assert::IsTrue(gl.HasEdge(2,1));
            Assert::IsTrue(gl.HasEdge(0,2));
            Assert::IsFalse(gl.HasEdge(0,0));
            
            // Right side graph
            auto gr = production.R_graph();
            Assert::AreEqual(3, gr.Size());
            Assert::AreEqual(105, gr[0].label);
            Assert::AreEqual(104, gr[1].label);
            Assert::AreEqual(102, gr[2].label);
            Assert::IsTrue(gr.HasEdge(0, 1));
            Assert::IsTrue(gr.HasEdge(1, 0));
            Assert::IsTrue(gr.HasEdge(0, 2));
            Assert::IsFalse(gr.HasEdge(0, 0));
            Assert::IsFalse(gr.HasEdge(1, 2));
            Assert::IsFalse(gr.HasEdge(2, 1));
        }
    };
}