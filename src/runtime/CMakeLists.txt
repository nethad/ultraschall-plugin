project(${ULTRASCHALL_RUNTIME_NAME} LANGUAGES C CXX VERSION 6.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PUBLIC_RUNTIME_INCLUDES
  ${PUBLIC_INCLUDE_PATH}/ultraschall/runtime.h
  ${PUBLIC_INCLUDE_PATH}/ultraschall/runtimeglobals.h
  ${PUBLIC_INCLUDE_PATH}/ultraschall/runtimemalloc.h
  ${PUBLIC_INCLUDE_PATH}/ultraschall/runtimesequentialstream.h
  ${PUBLIC_INCLUDE_PATH}/ultraschall/runtimesharedobject.h
  ${PUBLIC_INCLUDE_PATH}/ultraschall/runtimestream.h
  ${PUBLIC_INCLUDE_PATH}/ultraschall/runtimestring.h
  ${PUBLIC_INCLUDE_PATH}/ultraschall/runtimestringutilities.h
)

set(PRIVATE_RUNTIME_INCLUDES
  runtimeclass.h
  runtimecommon.h
)

set(PRIVATE_RUNTIME_SOURCES
  runtimeguid.cpp
  runtimesequentialstream.cpp
  runtimestream.cpp
  runtimestring.cpp
  runtimestringutilities.cpp
)

add_library(${ULTRASCHALL_RUNTIME_NAME} SHARED
  ${PUBLIC_RUNTIME_INCLUDES}
  ${PRIVATE_RUNTIME_INCLUDES}
  ${PRIVATE_RUNTIME_SOURCES}
)

set_target_properties(${ULTRASCHALL_RUNTIME_NAME} PROPERTIES LINKER_LANGUAGE CXX)
