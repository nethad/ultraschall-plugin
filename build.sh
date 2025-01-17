#!/bin/bash

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

source scripts/BuildTools.sh

TOOLS_DIRECTORY=`pwd`/tools
BUILD_DIRECTORY=`pwd`/build
BUILD_CONFIGURATION=Debug
CMAKE_EXTRA_ARGS=""

if [ "$1" = "--help" ]; then
  echo "Usage: build.sh [ --bootstrap | --rebuild | --clean | --cleanall ]"
  exit 0
elif [ "$1" = "--bootstrap" ]; then
  RemoveDirectory $BUILD_DIRECTORY
  RemoveDirectory $TOOLS_DIRECTORY
  source ./bootstrap.sh
  exit 0
elif [ "$1" = "--cleanall" ]; then
  RemoveDirectory $BUILD_DIRECTORY
  RemoveDirectory $TOOLS_DIRECTORY
  exit 0
elif [ "$1" = "--clean" ]; then
  RemoveDirectory $BUILD_DIRECTORY
  exit 0
elif [ "$1" == "--rebuild" ]; then
  CMAKE_EXTRA_ARGS="--clean-first"
elif [ "$1" == "--release" ]; then
  BUILD_CONFIGURATION=Release
fi

CMAKE_INSTALL_DIRECTORY=$TOOLS_DIRECTORY/cmake
CMAKE_INSTALL_FOUND=0
CMAKE_REQUIRED_VERSION="3.12.0"

echo "Looking for CMake $CMAKE_REQUIRED_VERSION. Checking system install..."
if [ $CMAKE_INSTALL_FOUND -eq 0 ]; then
  CMAKE_INSTALL_PATH=cmake
  if [ -x "$(command -v $CMAKE_INSTALL_PATH)" ]; then
    CMAKE_CURRENT_VERSION=`$CMAKE_INSTALL_PATH --version | sort -V | tail -n 1 | awk -v n=3 '{print $n}'`
    echo "Found CMake version $CMAKE_CURRENT_VERSION."
    CompareVersions $CMAKE_CURRENT_VERSION $CMAKE_REQUIRED_VERSION
    if [ ! $? -eq 2 ]; then
      CMAKE_INSTALL_FOUND=1
    fi
  fi
fi

if [ $CMAKE_INSTALL_FOUND -eq 0 ]; then
  echo "CMake $CMAKE_REQUIRED_VERSION does not seem to be installed on this system. Checking local install..."
  CMAKE_INSTALL_PATH=$CMAKE_INSTALL_DIRECTORY/bin/cmake
  if [ -x "$(command -v $CMAKE_INSTALL_PATH)" ]; then
    CMAKE_CURRENT_VERSION=`$CMAKE_INSTALL_PATH --version | sort -V | tail -n 1 | awk -v n=3 '{print $n}'`
    echo "Found CMake version $CMAKE_CURRENT_VERSION."
    CompareVersions $CMAKE_CURRENT_VERSION $CMAKE_REQUIRED_VERSION
    if [ ! $? -eq 2 ]; then
      CMAKE_INSTALL_FOUND=1
      export PATH=$CMAKE_INSTALL_DIRECTORY/bin:$PATH
    fi
  fi
fi

if [ $CMAKE_INSTALL_FOUND -ne 0 ]; then
  if [ ! -d $BUILD_DIRECTORY ]; then
    mkdir $BUILD_DIRECTORY
  fi

  echo "Entering build directory..."
  pushd $BUILD_DIRECTORY > /dev/null

  echo "Configuring projects using $CMAKE_GENERATOR..."
  cmake -G"Unix Makefiles" -Wno-dev --warn-uninitialized --warn-unused-vars -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION ../
  if [ $? -ne 0 ]; then
    echo "Failed to configure projects."
    exit -1
  fi
  echo "Done."

  echo "Building projects using $CMAKE_GENERATOR..."
  cmake --build . $CMAKE_EXTRA_ARGS --target reaper_ultraschall --config $BUILD_CONFIGURATION -j 8
  if [ $? -ne 0 ]; then
    echo "Failed to build projects."
    exit -1
  fi
  echo "Done."

  echo "Leaving build directory..."
  popd > /dev/null
fi

exit 0
