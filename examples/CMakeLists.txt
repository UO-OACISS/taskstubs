
add_executable(main_c_without main.c)
add_executable(main_c_with main.c)
target_link_libraries(main_c_with timer_plugin)
target_compile_definitions(main_c_with PRIVATE TASKTIMER_USE_TIMERS)

add_executable(main_cpp_without main.cpp)
add_executable(main_cpp_with main.cpp)
target_link_libraries(main_cpp_with timer_plugin)
target_compile_definitions(main_cpp_with PRIVATE TASKTIMER_USE_TIMERS)

add_test(NAME testWithout_c COMMAND main_c_without)
add_test(NAME testWithout_c2 COMMAND main_c_with)
add_test(NAME testWith_c COMMAND main_c_with)
set_property(TEST testWith_c PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/tool_example/libMyToolC${CMAKE_SHARED_LIBRARY_SUFFIX}")

add_test(NAME testWithout_cpp COMMAND main_cpp_without)
add_test(NAME testWithout_cpp2 COMMAND main_cpp_with)
add_test(NAME testWith_cpp COMMAND main_cpp_with)
set_property(TEST testWith_cpp PROPERTY ENVIRONMENT "TASKTIMER_EXTERNAL_TOOL_LIBRARY=${CMAKE_BINARY_DIR}/tool_example/libMyToolC${CMAKE_SHARED_LIBRARY_SUFFIX}")


