#
# Copyright (c) 2014-2021 Kevin Huck
# Copyright (c) 2014-2021 University of Oregon
#
# Distributed under the BSD 2-Clause Software License. (See accompanying
# file LICENSE)
#

# Create our measurement tool library
add_library(MyToolC SHARED my_tool.c)

# Our tool has a dependency on libdl.so
target_link_libraries(MyToolC dl)

# Our tool needs to find the plugin_timer includes
target_include_directories(MyToolC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../timer_plugin/)

# Make sure we export symbols that the plugin loader will look for
target_compile_definitions(MyToolC PRIVATE COMPILE_PLUGIN)

# Enable GNU extensions
target_compile_definitions(MyToolC PRIVATE _GNU_SOURCE)

# Make a CUDA NVTX example

if(DEFINED CUDATOOLKIT_HOME OR DEFINED ENV{CUDATOOLKIT_HOME})
    find_package(CUDAToolkit REQUIRED)
    set(CUDAToolkit_FOUND TRUE CACHE BOOL "CUDA Toolkit was found")
    #find_package(NVTX)
    add_library(nvtx_tool SHARED nvtx_tool.cpp)
    target_link_libraries(nvtx_tool PRIVATE nvToolsExt dl)
    target_include_directories(nvtx_tool PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../timer_plugin/)
    target_compile_definitions(nvtx_tool PRIVATE COMPILE_PLUGIN)
    target_compile_definitions(nvtx_tool PRIVATE _GNU_SOURCE)
endif()
