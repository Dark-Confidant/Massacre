#include "Universe.h"
#include "gfx/GLUtility.h"

#include "MassacreTypes.h"

template <> int mcr::glType<float> () { return GL_FLOAT;          }
template <> int mcr::glType<double>() { return GL_DOUBLE;         }
template <> int mcr::glType<int>   () { return GL_INT;            }
template <> int mcr::glType<uint>  () { return GL_UNSIGNED_INT;   }
template <> int mcr::glType<short> () { return GL_SHORT;          }
template <> int mcr::glType<ushort>() { return GL_UNSIGNED_SHORT; }
template <> int mcr::glType<char>  () { return GL_BYTE;           }
template <> int mcr::glType<byte>  () { return GL_UNSIGNED_BYTE;  }
template <> int mcr::glType<bool>  () { return GL_BOOL;           }
