cmake_minimum_required (VERSION 3.22)
project(plugin C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-fPIC)
set(CMAKE_EXE_LINKER_FLAGS -rdynamic)

enable_testing()

# The headers are all in timer_plugin
include_directories(${CMAKE_SOURCE_DIR}/timer_plugin)

# This directory has the timer instrumentation API that a library will use to instrument itself
add_subdirectory(timer_plugin)

# This directory has a demonstration tool that a measurement library would implement
add_subdirectory(tool_example)

# This directory has simple examples
add_subdirectory(examples)

