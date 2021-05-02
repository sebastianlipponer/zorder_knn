add_library(zorder_knn INTERFACE)

target_include_directories(zorder_knn INTERFACE
    include
)

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.19)
    target_sources(zorder_knn PRIVATE
        include/zorder_knn/less.hpp
    )
endif()
