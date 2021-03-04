################################################################################
#
# Copyright (c) The Ultraschall Project (https://ultraschall.fm)
#
# The MIT License (MIT)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
################################################################################

set(REAPER_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../include/reaper)

include_directories(
  ${LIBZ_INCLUDE_PATH}
  ${LIBCURL_INCLUDE_PATH}
  ${LIBTAG_INCLUDE_PATH}
  ${LIBEXPAT_INCLUDE_PATH}
  ${LIBWDL_INCLUDE_PATH}
  ${REAPER_INCLUDE_PATH}
)

set(EXTRA_INCLUDES)
set(EXTRA_SOURCES
  ${LIBZ_SOURCE_PATH}/contrib/minizip/ioapi.c
  ${LIBZ_SOURCE_PATH}/contrib/minizip/unzip.c
)
set_source_files_properties(${LIBZ_SOURCE_PATH}/contrib/minizip/ioapi.c PROPERTIES GENERATED TRUE)
set_source_files_properties(${LIBZ_SOURCE_PATH}/contrib/minizip/unzip.c PROPERTIES GENERATED TRUE)

include_directories(${CMAKE_CURRENT_LIST_DIR})

if(${ULTRASCHALL_TARGET_SYSTEM} STREQUAL "win32")
  set(EXTRA_SOURCES ${EXTRA_SOURCES} PluginResource.rc)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  set(EXTRA_LIBRARIES version.lib ws2_32.lib crypt32.lib wldap32.lib mfplat.lib mf.lib mfuuid.lib shlwapi.lib comctl32.lib comdlg32.lib rpcrt4.lib)
elseif(${ULTRASCHALL_TARGET_SYSTEM} STREQUAL "macos")
  add_compile_options(-mmacosx-version-min=10.11)
  set_source_files_properties(macos/Platform.cpp macos/PlatformFileDialog.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
  set(EXTRA_LIBRARIES "-framework AppKit" "-framework Carbon" "-framework IOKit" "-framework Security")
elseif(${ULTRASCHALL_TARGET_SYSTEM} STREQUAL "linux")
  include_directories(${LIBGTK3_INCLUDE_PATH})
  set(EXTRA_LIBRARIES ${LIBGTK3_LIBRARY_PATH})
endif()

add_library(reaper_ultraschall SHARED
  ${EXTRA_INCLUDES}
  ${EXTRA_SOURCES}
)

add_dependencies(reaper_ultraschall zlib-build)
add_dependencies(reaper_ultraschall curl-build)
add_dependencies(reaper_ultraschall taglib-build)
add_dependencies(reaper_ultraschall expat-build)
add_dependencies(reaper_ultraschall wdl-build)

target_link_libraries(reaper_ultraschall
  ${ULTRASCHALL_RUNTIME_NAME}
  ${ULTRASCHALL_MODEL_NAME}
  ${LIBZ_LIBRARY_PATH}
  ${LIBCURL_LIBRARY_PATH}
  ${LIBTAG_LIBRARY_PATH}
  ${LIBEXPAT_LIBRARY_PATH}
  ${EXTRA_LIBRARIES}
)

set_target_properties(reaper_ultraschall PROPERTIES PREFIX "")

add_subdirectory(runtime)
add_subdirectory(model)
add_subdirectory(platform)
add_subdirectory(framework)
add_subdirectory(reaper)
add_subdirectory(plugin)
