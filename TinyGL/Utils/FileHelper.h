#pragma once

#include <string>

namespace Tiny
{
    namespace FileHelper
    {
        _NODISCARD bool LoadFileToString(std::string &Result, const std::string &FilePath);
    }
};