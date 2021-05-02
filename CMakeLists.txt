cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

project(zorder_knn
    LANGUAGES CXX
    VERSION 1.0
)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLE "Build example" ON)

set(CMAKE_DEBUG_POSTFIX "d")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/.extern/install")

# Visual studio solution directories.
set_property(GLOBAL PROPERTY USE_FOLDERS on)

# Enable simultaneous compilation of source files for MSVC.
add_compile_options(
    $<$<CXX_COMPILER_ID:MSVC>:/MP>
)

# Put all executables and libraries into a common directory.
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH    "${PROJECT_BINARY_DIR}/bin")

add_subdirectory(zorder_knn)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
