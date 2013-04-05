#include "Universe.h"
#include "ComRegistry.h"

template<>
mcr::ComRegistry* mcr::ExternalSingleton<mcr::ComRegistry>::m_instance = nullptr;
