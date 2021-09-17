#pragma once

#include <string>

namespace Tiny
{
    namespace FileHelper
    {
        [[nodiscard]] bool LoadFileToString(std::string &Result, const std::string &FilePath);
    }
};