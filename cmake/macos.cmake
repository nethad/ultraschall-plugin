################################################################################
#
# Copyright (c) The Ultraschall Project (http://ultraschall.fm)
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

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "$ENV{HOME}/Library/Application\ Support/REAPER/UserPlugins")

include(ExternalProject)
set(CMAKE_GLOBAL_ARGS -Wno-dev -Wno-deprecated --warn-uninitialized warn-unused-vars -DCMAKE_OSX_DEPLOYMENT_TARGET=10.11 -DBUILD_SHARED_LIBS=OFF)

# configure zlib
set(CURRENT_EXTERNAL_PROJECT zlib)
message(STATUS "${CURRENT_EXTERNAL_PROJECT}<${CMAKE_BUILD_TYPE}>: Configuring...")
ExternalProject_Add(zlib
  PREFIX libz
  GIT_REPOSITORY https://github.com/madler/zlib.git
  GIT_TAG v1.2.11
  ${PROJECT_GLOBAL_ARGS}
  CMAKE_ARGS ${CMAKE_GLOBAL_ARGS}
)
ExternalProject_Get_Property(zlib SOURCE_DIR)
ExternalProject_Get_Property(zlib BINARY_DIR)
set(LIBZ_INCLUDE_PATH ${BINARY_DIR} ${SOURCE_DIR} ${SOURCE_DIR}/contrib/minizip)
set(LIBZ_LIBRARY_PATH ${BINARY_DIR}/libz.a)
set(LIBZ_SOURCE_PATH ${SOURCE_DIR})
message(STATUS "LIBZ_INCLUDE_PATH = ${LIBZ_INCLUDE_PATH}")
message(STATUS "LIBZ_LIBRARY_PATH = ${LIBZ_LIBRARY_PATH}")
message(STATUS "LIBZ_SOURCE_PATH  = ${LIBZ_SOURCE_PATH}")
message(STATUS "${CURRENT_EXTERNAL_PROJECT}<${CMAKE_BUILD_TYPE}>: Done.")

# configure curl
set(CURRENT_EXTERNAL_PROJECT curl)
message(STATUS "${CURRENT_EXTERNAL_PROJECT}<${CMAKE_BUILD_TYPE}>: Configuring...")
set(LIBCURL_BUILD_ARGS
  -DBUILD_SHARED_LIBS=OFF
  -DBUILD_CURL_EXE=OFF
  -DHTTP_ONLY=ON
  -DENABLE_IPV6=ON
  -DBUILD_TESTING=OFF
  -DBUILD_BINDINGS=OFF
  -DCMAKE_USE_DARWINSSL=ON
  -DCMAKE_USE_OPENSSL=OFF
  -DCMAKE_USE_LIBSSH2=OFF
  -DCURL_CA_PATH=none)
ExternalProject_Add(curl
  PREFIX libcurl
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG curl-7_63_0
  ${PROJECT_GLOBAL_ARGS}
  CMAKE_ARGS ${CMAKE_GLOBAL_ARGS} ${LIBCURL_BUILD_ARGS}
)
ExternalProject_Get_Property(curl SOURCE_DIR)
ExternalProject_Get_Property(curl BINARY_DIR)
set(LIBCURL_INCLUDE_PATH ${SOURCE_DIR}/include)
set(LIBCURL_LIBRARY_PATH ${BINARY_DIR}/lib/libcurl.a)
message(STATUS "LIBCURL_INCLUDE_PATH = ${LIBCURL_INCLUDE_PATH}")
message(STATUS "LIBCURL_LIBRARY_PATH = ${LIBCURL_LIBRARY_PATH}")
message(STATUS "${CURRENT_EXTERNAL_PROJECT}<${CMAKE_BUILD_TYPE}>: Done.")

# configure taglib
set(CURRENT_EXTERNAL_PROJECT taglib)
message(STATUS "${CURRENT_EXTERNAL_PROJECT}<${CMAKE_BUILD_TYPE}>: Configuring...")
set(LIBTAG_BUILD_ARGS -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_BINDINGS=OFF)
include(ExternalProject)
ExternalProject_Add(taglib
  PREFIX libtag
  GIT_REPOSITORY https://github.com/taglib/taglib.git
  GIT_TAG v1.11.1
  ${PROJECT_GLOBAL_ARGS}
  CMAKE_ARGS ${CMAKE_GLOBAL_ARGS} ${LIBTAG_BUILD_ARGS}
)
ExternalProject_Get_Property(taglib SOURCE_DIR)
ExternalProject_Get_Property(taglib BINARY_DIR)
set(LIBTAG_INCLUDE_PATH
    ${BINARY_DIR} # taglib_config.h
    ${SOURCE_DIR}/taglib
    ${SOURCE_DIR}/taglib/toolkit
    ${SOURCE_DIR}/taglib/mpeg
    ${SOURCE_DIR}/taglib/mpeg/id3v2
    ${SOURCE_DIR}/taglib/mpeg/id3v2/frames
)
set(LIBTAG_LIBRARY_PATH   ${BINARY_DIR}/taglib/libtag.a)
message(STATUS "LIBTAG_INCLUDE_PATH = ${LIBTAG_INCLUDE_PATH}")
message(STATUS "LIBTAG_LIBRARY_PATH = ${LIBTAG_LIBRARY_PATH}")
message(STATUS "${CURRENT_EXTERNAL_PROJECT}<${CMAKE_BUILD_TYPE}>: Done.")

# configure expat
set(CURRENT_EXTERNAL_PROJECT expat)
message(STATUS "${CURRENT_EXTERNAL_PROJECT}<${CMAKE_BUILD_TYPE}>: Configuring...")
set(LIBEXPAT_BUILD_ARGS
  -DEXPAT_BUILD_DOCS=OFF
  -DEXPAT_BUILD_EXAMPLES=OFF
  -DEXPAT_BUILD_FUZZERS=OFF
  -DEXPAT_BUILD_TESTS=OFF
  -DEXPAT_BUILD_TOOLS=OFF
  -DEXPAT_ENABLE_INSTALL=OFF
  -DEXPAT_SHARED_LIBS=OFF
)
include(ExternalProject)
ExternalProject_Add(expat
  PREFIX libexpat
  SOURCE_SUBDIR expat
  GIT_REPOSITORY https://github.com/libexpat/libexpat.git
  GIT_TAG R_2_2_9
  ${PROJECT_GLOBAL_ARGS}
  CMAKE_ARGS ${CMAKE_GLOBAL_ARGS} ${LIBEXPAT_BUILD_ARGS}
)
ExternalProject_Get_Property(expat SOURCE_DIR)
ExternalProject_Get_Property(expat BINARY_DIR)
set(LIBEXPAT_INCLUDE_PATH ${SOURCE_DIR})
set(LIBEXPAT_LIBRARY_PATH ${BINARY_DIR}/libexpat.a)
message(STATUS "LIBEXPAT_INCLUDE_PATH = ${LIBEXPAT_INCLUDE_PATH}")
message(STATUS "LIBEXPAT_LIBRARY_PATH = ${LIBEXPAT_LIBRARY_PATH}")
message(STATUS "${CURRENT_EXTERNAL_PROJECT}<${CMAKE_BUILD_TYPE}>: Done.")

# configure wdl
set(CURRENT_EXTERNAL_PROJECT wdl)
message(STATUS "${CURRENT_EXTERNAL_PROJECT}<${CMAKE_BUILD_TYPE}>: Configuring...")
ExternalProject_Add(wdl
  PREFIX libwdl
  SOURCE_SUBDIR wdl
  GIT_REPOSITORY https://github.com/justinfrankel/WDL.git
  ${PROJECT_GLOBAL_ARGS}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)
ExternalProject_Get_Property(wdl SOURCE_DIR)
set(LIBWDL_INCLUDE_PATH ${SOURCE_DIR})
set(LIBWDL_SOURCE_PATH ${SOURCE_DIR})
message(STATUS "LIBWDL_INCLUDE_PATH = ${LIBWDL_INCLUDE_PATH}")
message(STATUS "LIBWDL_SOURCE_PATH = ${LIBWDL_SOURCE_PATH}")
message(STATUS "${CURRENT_EXTERNAL_PROJECT}<${CMAKE_BUILD_TYPE}>: Done.")
