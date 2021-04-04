project(${ULTRASCHALL_RUNTIME_NAME} LANGUAGES C CXX VERSION 6.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLATFORM_INCLUDES
  platform.h
  platformendianess.h
  platformcommon.h
  platformfiledialog.h
  platformfilesystem.h
  platformglobals.h
)
set(PLATFORM_SOURCES
  ${ULTRASCHALL_TARGET_SYSTEM}/platformfiledialog.cpp
  ${ULTRASCHALL_TARGET_SYSTEM}/platformfilesystem.cpp
)
