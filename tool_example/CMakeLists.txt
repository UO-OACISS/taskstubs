
# Create our measurement tool library
add_library(MyToolC SHARED my_tool.c)

# Our tool has a dependency on libdl.so
target_link_libraries(MyToolC dl)

# Make sure we export symbols that the plugin loader will look for
target_compile_definitions(MyToolC PRIVATE COMPILE_PLUGIN)

# Enable GNU extensions
target_compile_definitions(MyToolC PRIVATE _GNU_SOURCE)


