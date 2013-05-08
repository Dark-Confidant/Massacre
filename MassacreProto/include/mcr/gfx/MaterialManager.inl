namespace mcr {
namespace gfx {

//////////////////////////////////////////////////////////////////////////
// Construction/destruction

inline MaterialManager::MaterialManager():
    m_fs(new FileSystem), m_ownFs(true)
{
    _init();
}

inline MaterialManager::MaterialManager(FileSystem* fs):
    m_fs(fs), m_ownFs(false),
    m_preprocessor()
{
    _init();
}

inline MaterialManager::~MaterialManager()
{
    _destroy();

    if (m_ownFs)
        delete m_fs;
}


//////////////////////////////////////////////////////////////////////////
// Parameter buffer management

inline uint MaterialManager::requestParameterBufferBinding()
{
    auto binding = m_pb.nextFreeBinding;
    m_pb.nextFreeBinding = (m_pb.nextFreeBinding + 1) % m_pb.numBindings;
    return binding;
}

inline bool MaterialManager::addParameterBuffer(MaterialParameterBuffer* buffer)
{
    return addParameterBuffer(buffer->name(), buffer);
}

inline bool MaterialManager::addParameterBuffer(const std::string& name, MaterialParameterBuffer* buffer)
{
    if (!m_pb.buffers.insert(std::make_pair(name, buffer)).second)
        return false;

    buffer->init();
    return true;
}

inline void MaterialManager::removeParameterBuffer(MaterialParameterBuffer* buffer)
{
    removeParameterBuffer(buffer->name());
}

inline void MaterialManager::removeParameterBuffer(const std::string& name)
{
    m_pb.buffers.erase(name);
}

inline MaterialParameterBuffer* MaterialManager::parameterBuffer(const std::string& name) const
{
    auto it = m_pb.buffers.find(name);
    return it != m_pb.buffers.end() ? it->second : nullptr;
}


//////////////////////////////////////////////////////////////////////////
// Texture unit management

inline uint MaterialManager::requestTexUnit()
{
    auto unit = (uint) m_tex.nextFreeUnit;
    m_tex.nextFreeUnit = (m_tex.nextFreeUnit + 1) % m_tex.units.size();
    return unit;
}


//////////////////////////////////////////////////////////////////////////
// Accessors

inline FileSystem* MaterialManager::fs() const
{
    return m_fs;
}

inline bool MaterialManager::hasOwnFs() const
{
    return m_ownFs;
}


//////////////////////////////////////////////////////////////////////////
// Collection helpers

template <typename M>
inline void MaterialManager::_dropAll(M& map)
{
    for (auto it = map.begin(); it != map.end(); ++it)
        if (it->second->drop())
            it = map.erase(it);
}

template <typename M>
inline void MaterialManager::_grabAll(M& map)
{
    for (auto it = map.begin(); it != map.end(); ++it)
        it->second->grab();
}

} // ns gfx
} // ns mcr
