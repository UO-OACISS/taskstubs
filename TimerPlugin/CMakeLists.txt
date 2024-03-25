add_library(TimerPlugin Plugin.cpp TaskTimer.cpp)
target_link_libraries(TimerPlugin dl)
target_compile_definitions(TimerPlugin PRIVATE _GNU_SOURCE)


