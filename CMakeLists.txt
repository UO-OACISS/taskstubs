cmake_minimum_required (VERSION 3.22)

project(plugin CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
#add_compile_options(-Wno-return-type-c-linkage)

add_subdirectory(TimerPlugin)
add_subdirectory(ToolExample)

add_executable(main main.cpp)
target_link_libraries(main TimerPlugin)

enable_testing()
add_test(NAME testWithout COMMAND main)
add_test(NAME testWith COMMAND main)
set_property(TEST testWith PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/ToolExample/libMyTool${CMAKE_SHARED_LIBRARY_SUFFIX}")


