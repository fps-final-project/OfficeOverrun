#include "pch.h"
#include "CppUnitTest.h"
#include "GraphProductionReader.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include "winrt/Windows.Storage.h"
#include <regex>

namespace fs = std::filesystem;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WorldGenerator;
using namespace winrt;
using namespace winrt::Windows::Storage;

namespace FirstPersonShooter_WorldGeneration_Test
{
    TEST_CLASS(GraphProductionReaderTest)
    {
    public:
        TEST_METHOD(ReadGraphProduction_ReadsGraph)
        {
            //hstring folderPath = to_hstring(R"(C:\Projects\Inzynierka\FirstPersonShooter.WorldGeneration.Test\TestFiles\GraphProductionReaderTest\TestCase1.txt)");
            //hstring h_filePath = to_hstring(R"(C:\Projects\Inzynierka\FirstPersonShooter.WorldGeneration.Test\TestFiles\GraphProductionReaderTest\TestCase1.txt)");
            //StorageFolder folder = nullptr;
            //StorageFile s_file = nullptr;
            //try
            //{
            //    //s_file = StorageFile::GetFileFromPathAsync(h_filePath.c_str()).get();
            //    folder = StorageFolder::GetFolderFromPathAsync(folderPath.c_str()).as<StorageFolder>();
            //}
            //catch (std::exception ex)
            //{
            //}

            auto production = GraphProductionReader::ReadGraphProduction<void*>("");
        }
    };
}