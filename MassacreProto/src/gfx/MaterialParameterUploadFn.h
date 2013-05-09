// Internal header
#pragma once

#include <boost/preprocessor/seq/for_each.hpp>
#include <mcr/Types.h>

//////////////////////////////////////////////////////////////////////////
// Parameter type list, must match MaterialParameterType::Type

#define MCR_UPLOAD_AUXARGS_VEC() 1,
#define MCR_UPLOAD_AUXARGS_MAT() 1, GL_FALSE,

#define MCR_PARAMS                                      \
    ((float)  (1f)         (BOOST_PP_EMPTY))            \
    ((double) (1d)         (BOOST_PP_EMPTY))            \
    ((int)    (1i)         (BOOST_PP_EMPTY))            \
    ((uint)   (1ui)        (BOOST_PP_EMPTY))            \
    ((vec2)   (2fv)        (MCR_UPLOAD_AUXARGS_VEC))    \
    ((dvec2)  (2dv)        (MCR_UPLOAD_AUXARGS_VEC))    \
    ((ivec2)  (2iv)        (MCR_UPLOAD_AUXARGS_VEC))    \
    ((uvec2)  (2uiv)       (MCR_UPLOAD_AUXARGS_VEC))    \
    ((vec3)   (3fv)        (MCR_UPLOAD_AUXARGS_VEC))    \
    ((dvec3)  (3dv)        (MCR_UPLOAD_AUXARGS_VEC))    \
    ((ivec3)  (3iv)        (MCR_UPLOAD_AUXARGS_VEC))    \
    ((uvec3)  (3uiv)       (MCR_UPLOAD_AUXARGS_VEC))    \
    ((vec4)   (4fv)        (MCR_UPLOAD_AUXARGS_VEC))    \
    ((dvec4)  (4dv)        (MCR_UPLOAD_AUXARGS_VEC))    \
    ((ivec4)  (4iv)        (MCR_UPLOAD_AUXARGS_VEC))    \
    ((uvec4)  (4uiv)       (MCR_UPLOAD_AUXARGS_VEC))    \
    ((mat4)   (Matrix4fv)  (MCR_UPLOAD_AUXARGS_MAT))    \
    ((dmat4)  (Matrix4dv)  (MCR_UPLOAD_AUXARGS_MAT))

namespace mcr    {
namespace gfx    {
namespace detail {
namespace        {

template <typename T, bool Direct>
void uploadParameter(uint program, int loc, const void* mem);


//////////////////////////////////////////////////////////////////////////
// Indirect parameter upload, assuming prior glUseProgram call

#define MCR_UPLOAD_FN_SPEC(r, data, t)                          \
    template <>                                                 \
    void uploadParameter<BOOST_PP_SEQ_ELEM(0, t), false>        \
        (uint program, int loc, const void* mem)                \
    {                                                           \
        (void) program;                                         \
        BOOST_PP_CAT(glUniform, BOOST_PP_SEQ_ELEM(1, t))(       \
            loc, BOOST_PP_SEQ_ELEM(2, t)()                      \
            *static_cast<const BOOST_PP_SEQ_ELEM(0, t)*>(mem)); \
    }

BOOST_PP_SEQ_FOR_EACH(MCR_UPLOAD_FN_SPEC, ~, MCR_PARAMS)

#undef MCR_UPLOAD_FN_SPEC


//////////////////////////////////////////////////////////////////////////
// Direct parameter upload via EXT_direct_state_access functions

#define MCR_UPLOAD_FN_SPEC(r, data, t)                              \
    template <>                                                     \
    void uploadParameter<BOOST_PP_SEQ_ELEM(0, t), true>             \
        (uint program, int loc, const void* mem)                    \
    {                                                               \
        BOOST_PP_CAT(glProgramUniform, BOOST_PP_SEQ_ELEM(1, t))(    \
            program, loc, BOOST_PP_SEQ_ELEM(2, t)()                 \
            *static_cast<const BOOST_PP_SEQ_ELEM(0, t)*>(mem));     \
    }

BOOST_PP_SEQ_FOR_EACH(MCR_UPLOAD_FN_SPEC, ~, MCR_PARAMS)

#undef MCR_UPLOAD_FN_SPEC

} // ns
} // ns detail
namespace {

//////////////////////////////////////////////////////////////////////////
// Upload function tables,
// choose one according to presence of the above extension

#define MCR_UPLOAD_FN_ENTRY_SPEC(r, direct, t) \
    &detail::uploadParameter<BOOST_PP_SEQ_ELEM(0, t), direct>,

void(*const g_uploadFnTableIndirect[])(uint, int, const void*) =
{
    BOOST_PP_SEQ_FOR_EACH(MCR_UPLOAD_FN_ENTRY_SPEC, false, MCR_PARAMS)
};

void(*const g_uploadFnTableDirect[])(uint, int, const void*) =
{
    BOOST_PP_SEQ_FOR_EACH(MCR_UPLOAD_FN_ENTRY_SPEC, true, MCR_PARAMS)
};

#undef MCR_UPLOAD_FN_ENTRY_SPEC


#undef MCR_PARAMS
#undef MCR_UPLOAD_AUXARGS_MAT
#undef MCR_UPLOAD_AUXARGS_VEC

} // ns
} // ns gfx
} // ns mcr
