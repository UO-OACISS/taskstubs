cmake_minimum_required (VERSION 3.22)

project(plugin C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
#add_compile_options(-Wno-return-type-c-linkage)
add_compile_options(-fPIC)
set(CMAKE_EXE_LINKER_FLAGS -rdynamic)

add_subdirectory(TimerPlugin)
add_subdirectory(ToolExample)

add_executable(main_cpp main.cpp)
target_link_libraries(main_cpp TimerPlugin)

add_executable(main_c main.c)
target_link_libraries(main_c TimerPlugin)

enable_testing()
add_test(NAME testWithout_cpp COMMAND main_cpp)
add_test(NAME testWith_cpp COMMAND main_cpp)
set_property(TEST testWith_cpp PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/ToolExample/libMyTool${CMAKE_SHARED_LIBRARY_SUFFIX}")

add_test(NAME testWithout_c COMMAND main_c)
add_test(NAME testWith_c COMMAND main_c)
set_property(TEST testWith_c PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/ToolExample/libMyTool${CMAKE_SHARED_LIBRARY_SUFFIX}")


