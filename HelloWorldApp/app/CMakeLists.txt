# Copyright (c) Meta Platforms, Inc. and affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

cmake_minimum_required(VERSION 3.4.1)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-DFOLLY_NO_CONFIG=1 -DFOLLY_HAVE_CLOCK_GETTIME=1 -DFOLLY_HAVE_MEMRCHR=1 -DFOLLY_USE_LIBCPP=1 -DFOLLY_MOBILE=1 -DON_ANDROID -DONANDROID")

set (PACKAGE_NAME "torch_test")
set (BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

# PyTorch Core shared

add_library(
        ${PACKAGE_NAME}
        SHARED
        src/main/cpp/OnLoad.cpp
)

# includes

file (GLOB LIBFBJNI_INCLUDE_DIRS "${BUILD_DIR}/fbjni-*-headers.jar/")
file (GLOB PYTORCH_INCLUDE_DIRS "${BUILD_DIR}/pytorch_android_lite*.aar/headers")

target_include_directories(
        ${PACKAGE_NAME}
        PRIVATE
        "${LIBFBJNI_INCLUDE_DIRS}"
        "${PYTORCH_INCLUDE_DIRS}"
        "src/main/cpp"
)

# find libraries

file (GLOB PYTORCH_LINK_DIRS "${BUILD_DIR}/pytorch_android_lite*.aar/jni/${ANDROID_ABI}")

find_library(
        LOG_LIB
        log
)

find_library(
        FBJNI_LIBRARY
        fbjni
        PATHS ${PYTORCH_LINK_DIRS}
        NO_CMAKE_FIND_ROOT_PATH
)

find_library(
        PYTORCH_LIBRARY
        pytorch_jni_lite
        PATHS ${PYTORCH_LINK_DIRS}
        NO_CMAKE_FIND_ROOT_PATH
)

# linking

target_link_libraries(
        ${PACKAGE_NAME}
        ${LOG_LIB}
        ${PYTORCH_LIBRARY}
        ${FBJNI_LIBRARY}
        android
)
