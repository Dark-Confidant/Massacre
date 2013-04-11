cmake_minimum_required(VERSION 2.6)

set(HEADERS
  include/MassacreTypes.h
  include/MassacreBuildConfig.h
  include/NonCopyable.h
  include/RefCounted.h)
set(HEADERS_BB
  include/bb/Manips.h
  include/bb/ByteBuffer.h)
set(HEADERS_MATH
  include/math/Expr.h
  include/math/Grid.h
  include/math/Rect.h
  include/math/Vector.h
  include/math/Math.h
  include/math/Matrix.h)

install(FILES ${HEADERS} 
        DESTINATION include/massacre)
install(FILES ${HEADERS_BB} 
        DESTINATION include/massacre/bb)
install(FILES ${HEADERS_MATH} 
        DESTINATION include/massacre/math)
