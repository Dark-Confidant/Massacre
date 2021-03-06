#pragma once

namespace mcr {
namespace gfx {
namespace     {

const GLenum g_depthFnTable[] =
{
    GL_NEVER,
    GL_LESS,
    GL_EQUAL,
    GL_LEQUAL,
    GL_GREATER,
    GL_NOTEQUAL,
    GL_GEQUAL,
    GL_ALWAYS
};

const GLenum g_blendFnTable[] =
{
    GL_ZERO, GL_ONE,
    GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
    GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA,
    GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
    GL_SRC_ALPHA_SATURATE
};

const GLenum g_bufferTypeTable[] =
{
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
    GL_UNIFORM_BUFFER,
    GL_TEXTURE_BUFFER,
    GL_TRANSFORM_FEEDBACK_BUFFER

    // TODO: support
    /*GL_ATOMIC_COUNTER_BUFFER,
    GL_COPY_READ_BUFFER,
    GL_COPY_WRITE_BUFFER,
    GL_DRAW_INDIRECT_BUFFER,
    GL_DISPATCH_INDIRECT_BUFFER,
    GL_PIXEL_PACK_BUFFER,
    GL_PIXEL_UNPACK_BUFFER,
    GL_SHADER_STORAGE_BUFFER*/
};

const GLenum g_bufferUsageTable[] =
{
    GL_STREAM_DRAW,  GL_STREAM_READ,  GL_STREAM_COPY,
    GL_STATIC_DRAW,  GL_STATIC_READ,  GL_STATIC_COPY,
    GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY
};

const GLbitfield g_mapAccessTable[] =
{
    0,
    GL_MAP_READ_BIT,
    GL_MAP_WRITE_BIT,
    GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_INVALIDATE_RANGE_BIT,
    GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_READ_BIT,
    GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_INVALIDATE_BUFFER_BIT,
    GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_READ_BIT,
    GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT,
    GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_READ_BIT,
    GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_READ_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_READ_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_READ_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_READ_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_READ_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_READ_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_READ_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_READ_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT,
    GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT
};

GLenum toLegacyGLMapAccess(GLbitfield access)
{
    switch (access & (GL_MAP_READ_BIT | GL_MAP_WRITE_BIT))
    {
    case GL_MAP_READ_BIT:  return GL_READ_ONLY;
    case GL_MAP_WRITE_BIT: return GL_WRITE_ONLY;
    default:               return GL_READ_WRITE;
    }
}

const GLenum g_primitiveTypeTable[] =
{
    GL_POINT,
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP,
    GL_TRIANGLE_FAN,
    GL_QUADS
};

const GLenum g_attribTypeTable[] =
{
    GL_NONE,
    GL_BYTE,  GL_UNSIGNED_BYTE,
    GL_SHORT, GL_UNSIGNED_SHORT,
    GL_INT,   GL_UNSIGNED_INT,
    GL_FLOAT, GL_DOUBLE
};

geom::AttribType toAttribType(GLenum glAttribType)
{
    switch (glAttribType)
    {
    case GL_BYTE:           return geom::AttribType::SByte;
    case GL_UNSIGNED_BYTE:  return geom::AttribType::Byte;
    case GL_SHORT:          return geom::AttribType::Short;
    case GL_UNSIGNED_SHORT: return geom::AttribType::UShort;
    case GL_INT:            return geom::AttribType::Int;
    case GL_UNSIGNED_INT:   return geom::AttribType::UInt;
    case GL_FLOAT:          return geom::AttribType::Float;
    case GL_DOUBLE:         return geom::AttribType::Double;
    default:                return geom::AttribType::Unknown;
    }
}

} // ns
} // ns gfx
} // ns mcr
