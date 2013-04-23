#include "Universe.h"
#include <mcr/gfx/GLUtility.h>

#include <mcr/MassacreTypes.h>

namespace mcr {

template <> int glType<float> () { return GL_FLOAT;          }
template <> int glType<double>() { return GL_DOUBLE;         }
template <> int glType<int>   () { return GL_INT;            }
template <> int glType<uint>  () { return GL_UNSIGNED_INT;   }
template <> int glType<short> () { return GL_SHORT;          }
template <> int glType<ushort>() { return GL_UNSIGNED_SHORT; }
template <> int glType<char>  () { return GL_BYTE;           }
template <> int glType<byte>  () { return GL_UNSIGNED_BYTE;  }
template <> int glType<bool>  () { return GL_BOOL;           }

} // ns mcr
