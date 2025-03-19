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

# This directory has a demonstration tool that a measurement
# library would implement
add_subdirectory(tool_examples)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # If we're configuring this project standalone, we want testers
  # and examples
  enable_testing()

  # This directory has simple examples
  add_subdirectory(examples)
endif (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

## All the installation things go here ##

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
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
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
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

    # pkg-config files
    configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/share/pkgconfig/${PROJECT_NAME}.pc.in
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
	@ONLY
     )
     
    install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
    )
endif (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

# generate the export targets for the build tree
# (can't say what this one is for, but so far it has been only causing me problems,
# so I stopped adding it to projects)
# export(EXPORT "${PROJECT_NAME}Targets"
#     FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Targets.cmake"
#     NAMESPACE ${namespace}::
#)

function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        if (ARGV0)
            unset(MATCHED)
            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
            if (NOT MATCHED)
                continue()
            endif()
        endif()
        message(STATUS "${_variableName} = ${${_variableName}}")
    endforeach()
endfunction()

function(dump_defines)
    get_directory_property( DirDefs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )
    foreach( d ${DirDefs} )
        message( STATUS "Found Define: " ${d} )
    endforeach()
endfunction()

message(STATUS "----------------------------------------------------------------------")
message(STATUS "TaskStubs Variable Report:")
message(STATUS "----------------------------------------------------------------------")
dump_cmake_variables("^TASKSTUBS")
message(STATUS "----------------------------------------------------------------------")
message(STATUS "APEX Dependency Report:")
message(STATUS "----------------------------------------------------------------------")
dump_cmake_variables("^CUDAToolkit")
dump_cmake_variables("^ROC")
dump_defines()
MESSAGE(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
MESSAGE(STATUS "Libraries: " ${LIBS})
MESSAGE(STATUS "Compiler cxx debug flags:" ${CMAKE_CXX_FLAGS_DEBUG})
MESSAGE(STATUS "Compiler cxx release flags:" ${CMAKE_CXX_FLAGS_RELEASE})
MESSAGE(STATUS "Compiler cxx min size flags:" ${CMAKE_CXX_FLAGS_MINSIZEREL})
MESSAGE(STATUS "Compiler cxx flags:" ${CMAKE_CXX_FLAGS})
MESSAGE(STATUS "Install Prefix:" ${CMAKE_INSTALL_PREFIX})
message(STATUS "----------------------------------------------------------------------")

