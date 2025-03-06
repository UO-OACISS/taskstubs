#
# Copyright (c) 2014-2021 Kevin Huck
# Copyright (c) 2014-2021 University of Oregon
#
# Distributed under the BSD 2-Clause Software License. (See accompanying
# file LICENSE)
#

# Simple example in C, instrumentation enabled
add_executable(main_c_with main.c)
target_compile_definitions(main_c_with PRIVATE TASKTIMER_USE_TIMERS)
# Simple example in C, dependency on plugin loader library
target_link_libraries(main_c_with timer_plugin)

# Simple example in C++, instrumentation enabled
add_executable(main_cpp_with main.cpp)
target_compile_definitions(main_cpp_with PRIVATE TASKTIMER_USE_TIMERS)
# Simple example in C++, dependency on plugin loader library
target_link_libraries(main_cpp_with timer_plugin)

# Test with plugin support, but no tool
add_test(NAME testWithout_tool_c COMMAND main_c_with)
# Test with plugin support and tool loaded
add_test(NAME testWith_tool_c COMMAND main_c_with)
set_property(TEST testWith_tool_c PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/tool_examples/libMyToolC${CMAKE_SHARED_LIBRARY_SUFFIX}")

if(CUDAToolkit_FOUND)
# Test with plugin support and nvtx tool loaded
add_test(NAME testWith_nvtx_tool_c COMMAND main_c_with)
set_property(TEST testWith_nvtx_tool_c PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/tool_examples/libnvtx_tool${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

if(ROCTX_FOUND)
# Test with plugin support and nvtx tool loaded
add_test(NAME testWith_roctx_tool_c COMMAND main_c_with)
set_property(TEST testWith_roctx_tool_c PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/tool_examples/libroctx_tool${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

# Test with plugin support, but no tool
add_test(NAME testWithout_tool_cpp COMMAND main_cpp_with)
# Test with plugin support and tool loaded
add_test(NAME testWith_tool_cpp COMMAND main_cpp_with)
set_property(TEST testWith_tool_cpp PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/tool_examples/libMyToolC${CMAKE_SHARED_LIBRARY_SUFFIX}")

if(CUDAToolkit_FOUND)
# Test with plugin support and nvtx tool loaded
add_test(NAME testWith_nvtx_tool_cpp COMMAND main_cpp_with)
set_property(TEST testWith_nvtx_tool_cpp PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/tool_examples/libnvtx_tool${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

if(ROCTX_FOUND)
# Test with plugin support and roctx tool loaded
add_test(NAME testWith_roctx_tool_cpp COMMAND main_cpp_with)
set_property(TEST testWith_roctx_tool_cpp PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/tool_examples/libroctx_tool${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

