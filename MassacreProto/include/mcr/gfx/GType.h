#pragma once

#include <mcr/Types.h>

namespace mcr {
namespace gfx {

struct GType {
    enum Type: uint
    {
        Unknown,
        SByte, Byte,
        Short, UShort,
        Int,   UInt,
        Float, Double,
    }
    type;

    GType(Type atype = Unknown): type(atype) {}
    explicit GType(uint atype): type(Type(atype)) {}

    uint size() const
    {
        static const uint s_sizes[] = {0, 1, 1, 2, 2, 4, 4, 4, 8};
        return s_sizes[type];
    }

    bool valid() const
    {
        return Unknown < type && type <= Double;
    }

    MCR_EXTERN ushort toGLType() const;
    MCR_EXTERN static GType fromGLType(ushort glType);
};


inline bool operator==(const GType& lhs, const GType& rhs)
{
    return lhs.type == rhs.type;
}

inline bool operator!=(const GType& lhs, const GType& rhs)
{
    return !(lhs == rhs);
}

} // ns gfx
} // ns mcr
