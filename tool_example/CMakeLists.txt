
add_library(MyToolC SHARED my_tool.c)
target_compile_definitions(MyToolC PRIVATE COMPILE_PLUGIN)
target_compile_definitions(MyToolC PRIVATE _GNU_SOURCE)
target_link_libraries(MyToolC dl)

