cmake_minimum_required (VERSION 3.22)

project(plugin C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-fPIC)
set(CMAKE_EXE_LINKER_FLAGS -rdynamic)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/timer_plugin)

add_subdirectory(timer_plugin)
add_subdirectory(tool_example)
add_subdirectory(examples)

