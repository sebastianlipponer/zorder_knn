cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(build-extern
    LANGUAGES NONE
)

include(ExternalProject)

function(DownloadProject _name)
    cmake_parse_arguments(_arg "" "URL;URL_MD5" "" ${ARGN})

    if (NOT DEFINED _arg_URL)
        message(FATAL_ERROR "Error: No 'URL' argument given.")
    endif()
    if (NOT DEFINED _arg_URL_MD5)
        message(FATAL_ERROR "Error: No 'URL_MD5' argument given.")
    endif()

    ExternalProject_Add(${_name}
        URL          "${_arg_URL}"
        URL_MD5      "${_arg_URL_MD5}"
        LOG_DOWNLOAD TRUE

        STEP_TARGETS patch

        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""

        ${_arg_UNPARSED_ARGUMENTS}
    )

    string(TOUPPER "${_name}" _upper_name)
    ExternalProject_Get_Property(${_name} SOURCE_DIR)
    set(${_upper_name}_SOURCE_DIR "${SOURCE_DIR}" PARENT_SCOPE)
endfunction()

function(BuildProject _name)
    cmake_parse_arguments(_arg "" "BUILD_TYPE" "" ${ARGN})
    if (NOT DEFINED _arg_BUILD_TYPE)
        message(FATAL_ERROR "Error: No 'BUILD_TYPE' argument given.")
    endif()

    string(TOLOWER "${_arg_BUILD_TYPE}" _lower_build_type)
    string(TOUPPER "${_name}" _upper_name)

    ExternalProject_Add(${_name}-${_lower_build_type}
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${${_upper_name}_SOURCE_DIR}

        INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
        CMAKE_CACHE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>"
            "-DCMAKE_BUILD_TYPE:STRING=${_arg_BUILD_TYPE}"
            "-DCMAKE_FIND_ROOT_PATH:PATH=<INSTALL_DIR>"

        STEP_TARGETS install

        LOG_CONFIGURE TRUE
        LOG_BUILD TRUE
        LOG_INSTALL TRUE

        ${_arg_UNPARSED_ARGUMENTS}
    )

    ExternalProject_Add_StepDependencies(${_name}-${_lower_build_type}
         configure ${_name}-patch
    )
endfunction()

# Download.
DownloadProject(googletest
    URL     "https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip"
    URL_MD5 "b4911e882c51cba34bebfb5df500a650"
)

foreach(_build_type IN ITEMS Debug Release)
    BuildProject(googletest
        BUILD_TYPE ${_build_type}
        CMAKE_CACHE_ARGS "-Dgtest_force_shared_crt:BOOL=ON"
            "-DCMAKE_DEBUG_POSTFIX:STRING=d"
    )
endforeach()

# The install step of 'debug' and 'release' projects cannot be executed
# in parallel since they write common files.
foreach(_name IN ITEMS googletest)
    ExternalProject_Add_StepDependencies(${_name}-release
         install ${_name}-debug-install
    )
endforeach()
