cmake_minimum_required(VERSION 2.6)

set(HEADERS
  mcr/MassacreTypes.h
  mcr/MassacreBuildConfig.h
  mcr/NonCopyable.h
  mcr/RefCounted.h)
set(HEADERS_BB
  mcr/bb/Manips.h
  mcr/bb/ByteBuffer.h)
set(HEADERS_MATH
  mcr/math/Expr.h
  mcr/math/Grid.h
  mcr/math/Rect.h
  mcr/math/Vector.h
  mcr/math/Math.h
  mcr/math/Matrix.h)

install(FILES ${HEADERS} 
        DESTINATION include/mcr)
install(FILES ${HEADERS_BB} 
        DESTINATION include/mcr/bb)
install(FILES ${HEADERS_MATH} 
        DESTINATION include/mcr/math)
