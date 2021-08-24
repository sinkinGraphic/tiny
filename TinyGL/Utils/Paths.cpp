#include "Paths.h"

std::string Tiny::Paths::ProjectDir()
{
#ifdef PROJECT_DIR
    std::string Path = PROJECT_DIR;
            Path += "/../";
            return Path;
#else
    static_assert("Project path not configured");
#endif
}
