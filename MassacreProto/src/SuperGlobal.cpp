#include "Universe.h"
#include "SuperGlobal.h"
#include <unordered_map>

namespace
{
    typedef std::unordered_map<std::string, void*> SGMap;
    SGMap* g_sgMap;
}

void*& mcr::superglobal(const std::string& name)
{
    if (!g_sgMap)
        g_sgMap = new SGMap;

    return (*g_sgMap)[name];
}
