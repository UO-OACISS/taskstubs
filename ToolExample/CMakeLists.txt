include_directories(..)

add_library(MyToolCPP SHARED MyTool.cpp)
target_compile_definitions(MyToolCPP PRIVATE COMPILE_PLUGIN)

add_library(myToolC SHARED my_tool.c)
target_compile_definitions(myToolC PRIVATE COMPILE_PLUGIN)

