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
add_test(NAME testWithout_c2 COMMAND main_c_with)
# Test with plugin support and tool loaded
add_test(NAME testWith_c COMMAND main_c_with)
set_property(TEST testWith_c PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/tool_example/libMyToolC${CMAKE_SHARED_LIBRARY_SUFFIX}")

# Test with plugin support, but no tool
add_test(NAME testWithout_cpp2 COMMAND main_cpp_with)
# Test with plugin support and tool loaded
add_test(NAME testWith_cpp COMMAND main_cpp_with)
set_property(TEST testWith_cpp PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/tool_example/libMyToolC${CMAKE_SHARED_LIBRARY_SUFFIX}")


