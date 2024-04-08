# Include this directory when searching for headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create the library with two source files
add_library(timer_plugin tasktimer.c plugin.c)

# There is a dependency on libdl.so
target_link_libraries(timer_plugin dl)

# Enable GNU extensions
target_compile_definitions(timer_plugin PRIVATE _GNU_SOURCE)


