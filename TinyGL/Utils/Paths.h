#pragma once

#include <string>

namespace Tiny
{
    namespace Paths
    {
        _NODISCARD std::string ProjectDir()
        {
#ifdef PROJECT_DIR
            std::string Path = PROJECT_DIR;
            Path += "/";
            return Path;
#else
            static_assert("Project path not configured");
#endif
        }
    };
};