namespace mcr {
namespace gfx {
namespace mtl {

//////////////////////////////////////////////////////////////////////////
// Construction/destruction

inline Manager::Manager():
    m_fs(new io::FileSystem), m_ownFs(true)
{
    _init();
}

inline Manager::Manager(io::FileSystem* fs):
    m_fs(fs), m_ownFs(false),
    m_preprocessor()
{
    _init();
}

inline Manager::~Manager()
{
    _destroy();

    if (m_ownFs)
        delete m_fs;
}


//////////////////////////////////////////////////////////////////////////
// Parameter buffer management

inline uint Manager::requestParamBufferBinding()
{
    auto binding = m_pb.nextFreeBinding;
    m_pb.nextFreeBinding = (m_pb.nextFreeBinding + 1) % m_pb.numBindings;
    return binding;
}

inline bool Manager::addParamBuffer(mtl::ParamBuffer* buffer)
{
    return addParamBuffer(buffer->name(), buffer);
}

inline bool Manager::addParamBuffer(const std::string& name, mtl::ParamBuffer* buffer)
{
    if (!m_pb.buffers.insert(std::make_pair(name, buffer)).second)
        return false;

    buffer->init();
    return true;
}

inline void Manager::removeParamBuffer(mtl::ParamBuffer* buffer)
{
    removeParamBuffer(buffer->name());
}

inline void Manager::removeParamBuffer(const std::string& name)
{
    m_pb.buffers.erase(name);
}

inline mtl::ParamBuffer* Manager::paramBuffer(const std::string& name) const
{
    auto it = m_pb.buffers.find(name);
    return it != m_pb.buffers.end() ? it->second : nullptr;
}


//////////////////////////////////////////////////////////////////////////
// Texture unit management

inline uint Manager::requestTexUnit()
{
    auto unit = (uint) m_tex.nextFreeUnit;
    m_tex.nextFreeUnit = (m_tex.nextFreeUnit + 1) % m_tex.units.size();
    return unit;
}


//////////////////////////////////////////////////////////////////////////
// Accessors

inline io::FileSystem* Manager::fs() const
{
    return m_fs;
}

inline bool Manager::hasOwnFs() const
{
    return m_ownFs;
}


//////////////////////////////////////////////////////////////////////////
// Collection helpers

template <typename M>
inline void Manager::_dropAll(M& map)
{
    for (auto it = map.begin(); it != map.end(); ++it)
        if (it->second->drop())
            it = map.erase(it);
}

template <typename M>
inline void Manager::_grabAll(M& map)
{
    for (auto it = map.begin(); it != map.end(); ++it)
        it->second->grab();
}

} // ns mtl
} // ns gfx
} // ns mcr
