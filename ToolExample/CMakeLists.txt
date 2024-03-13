include_directories(..)

add_library(MyTool SHARED MyTool.cpp)
target_compile_definitions(MyTool PRIVATE COMPILE_PLUGIN)

