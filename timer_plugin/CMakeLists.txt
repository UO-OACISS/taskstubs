include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(timer_plugin tasktimer.c plugin.c)
target_link_libraries(timer_plugin dl)
target_compile_definitions(timer_plugin PRIVATE _GNU_SOURCE)


