#
# Copyright (c) 2014-2021 Kevin Huck
# Copyright (c) 2014-2021 University of Oregon
#
# Distributed under the BSD 2-Clause Software License. (See accompanying
# file LICENSE)
#

SET(TIMERLIB "timer_plugin")

# Create the library target
add_library(${TIMERLIB})

# and add the two source files to it
target_sources(${TIMERLIB} PRIVATE
  tasktimer.c plugin.c)

# We define the headers per target so CMake knows how to
# install things better
target_include_directories(${TIMERLIB} 
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}> 
)  

# There is a dependency on libdl.so
target_link_libraries(${TIMERLIB} dl)

# Enable GNU extensions to define RTLD_DEFAULT for the dlsym() call.
target_compile_definitions(${TIMERLIB} PRIVATE _GNU_SOURCE)


