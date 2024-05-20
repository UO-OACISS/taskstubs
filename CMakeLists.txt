#
# Copyright (c) 2014-2021 Kevin Huck
# Copyright (c) 2014-2021 University of Oregon
#
# Distributed under the BSD 2-Clause Software License. (See accompanying
# file LICENSE)
#

cmake_minimum_required (VERSION 3.22)
project(TASKSTUBS VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-fPIC)
set(CMAKE_EXE_LINKER_FLAGS -rdynamic)

# This directory has the timer instrumentation API that a library
# will use to instrument itself
add_subdirectory(timer_plugin)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # If we're configuring this project standalone, we want testers
  # and examples
  enable_testing()

  # This directory has a demonstration tool that a measurement
  # library would implement
  add_subdirectory(tool_example)

  # This directory has simple examples
  add_subdirectory(examples)
endif (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

## All the installation things go here ##

# for CMAKE_INSTALL_INCLUDEDIR definition
include(GNUInstallDirs)

set(public_headers
  timer_plugin/plugin.h
  timer_plugin/tasktimer.h
  timer_plugin/tool_api.h)

foreach(header ${public_headers})
    file(REAL_PATH "${header}" full_header)
    file(RELATIVE_PATH header_file_path "${CMAKE_CURRENT_SOURCE_DIR}" "${full_header}")
    get_filename_component(header_directory_path "${header_file_path}" DIRECTORY)
    install(
        FILES ${header}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${header_directory_path}"
    )
endforeach()

# install the target and create export-set
install(TARGETS timer_plugin
    EXPORT "${PROJECT_NAME}Targets"
    # these get default values from GNUInstallDirs, no need to set them
    #RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # bin
    #LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
    #ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
    # except for public headers, as we want them to be inside a library folder
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} # include/SomeLibrary
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # include
)

# generate and install export file
install(EXPORT "${PROJECT_NAME}Targets"
    FILE "${PROJECT_NAME}Targets.cmake"
    NAMESPACE ${namespace}::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)

# generate the version file for the config file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${TASKSTUBS_VERSION}"
    COMPATIBILITY AnyNewerVersion
)
# create config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION cmake
)
# install config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION cmake
)
# generate the export targets for the build tree
# (can't say what this one is for, but so far it has been only causing me problems,
# so I stopped adding it to projects)
# export(EXPORT "${PROJECT_NAME}Targets"
#     FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Targets.cmake"
#     NAMESPACE ${namespace}::
#)
