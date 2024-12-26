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
        }
    };
}