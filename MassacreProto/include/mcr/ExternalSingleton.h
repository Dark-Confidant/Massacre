#pragma once

namespace mcr {

template <class Super>
class ExternalSingleton
{
public:
    static Super& instance()
    {
        if (!m_instance)
            m_instance = new Super;

        return *m_instance;
    }

protected:
    ExternalSingleton<Super>() {}
    ExternalSingleton<Super>(const ExternalSingleton<Super>&);
    ExternalSingleton<Super>& operator=(const ExternalSingleton<Super>&);

    ~ExternalSingleton<Super>() {}

private:
    MCR_EXTERN static Super* m_instance; // to be defined
};

} // ns mcr
