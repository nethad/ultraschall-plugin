
set(PUBLIC_RUNTIME_INCLUDES
  ${PUBLIC_INCLUDE_PATH}/runtime.h
  ${PUBLIC_INCLUDE_PATH}/runtimeglobals.h
  ${PUBLIC_INCLUDE_PATH}/runtimemalloc.h
  ${PUBLIC_INCLUDE_PATH}/runtimesequentialstream.h
  ${PUBLIC_INCLUDE_PATH}/runtimesharedobject.h
  ${PUBLIC_INCLUDE_PATH}/runtimestream.h
  ${PUBLIC_INCLUDE_PATH}/runtimestring.h
  ${PUBLIC_INCLUDE_PATH}/runtimestringutilities.h
)

set(PRIVATE_RUNTIME_INCLUDES
  runtimeclass.h
  runtimecommon.h
)

set(PRIVATE_RUNTIME_SOURCES
  runtimesequentialstream.cpp
  runtimestream.cpp
  runtimestring.cpp
  runtimestringutilities.cpp
)

add_library(${ULTRASCHALL_RUNTIME_NAME} SHARED
  ${PUBLIC_RUNTIME_INCLUDES}
  ${PRIVATE_RUNTIME_INCLUDES}
  ${PRIVATE_RUNTIME_INCLUDES}
)

set_target_properties(${ULTRASCHALL_RUNTIME_NAME} PROPERTIES LINKER_LANGUAGE CXX)
