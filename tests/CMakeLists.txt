find_package(GTest REQUIRED CONFIG)

add_executable(test)
set_target_properties(test PROPERTIES FOLDER "Tests")

target_sources(test PRIVATE
    flt.cpp
    log2.cpp
    xor_msb.cpp
    less.cpp
)

target_link_libraries(test
    GTest::gtest GTest::gtest_main zorder_knn
)
