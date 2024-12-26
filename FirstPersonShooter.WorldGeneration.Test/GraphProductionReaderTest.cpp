#include "pch.h"
#include "CppUnitTest.h"
#include "GraphProductionReader.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WorldGenerator;

namespace FirstPersonShooter_WorldGeneration_Test
{
    TEST_CLASS(GraphProductionReaderTest)
    {
    public:
        TEST_METHOD(ReadGraphProduction_ReadsGraph)
        {

            std::ifstream file(R"(C:\Projects\Inzynierka\FirstPersonShooter.WorldGeneration.Test\TestFiles\GraphProductionReaderTest\TestCase1.txt)");
            //std::ifstream file(path, std::ios::in);

            if (!file.is_open())
                throw new std::exception("Failed to open file.");

            fs::path prev = "C:\\Projects\\Inzynierka\\FirstPersonShooter.WorldGeneration.Test\\Test Files\\GraphProductionReaderTest\\TestCase1.txt";

            auto wd = fs::current_path();
            auto wd2 = __FILE__;

            std::filesystem::path dirPath = std::filesystem::path(__FILE__).remove_filename();
            std::filesystem::path filePath = dirPath.concat("TestFiles\\GraphProductionReaderTest\\TestCase1.txt");

            auto production = GraphProductionReader::ReadGraphProduction<void*>(filePath.string());

        }
    };
}