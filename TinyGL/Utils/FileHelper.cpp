#include "FileHelper.h"
#include <fstream>
#include <memory>

bool Tiny::FileHelper::LoadFileToString(std::string &Result, const std::string &FilePath)
{
    std::ifstream File;
    File.open(FilePath.c_str(), std::ios::out);
    if (File)
    {
        File.seekg(0, std::ios::end);
        std::size_t FileSize = static_cast<size_t>(File.tellg());
        File.seekg(0, std::ios::beg);
        Result.clear();
        Result.resize(FileSize, 0);
        File.read(&Result[0], FileSize);
        return true;
    }
    return false;
}
