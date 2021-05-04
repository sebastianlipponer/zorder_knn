find_package(GTest REQUIRED CONFIG)
include(GoogleTest)

add_executable(unit_tests)
set_target_properties(unit_tests PROPERTIES FOLDER "Tests")

target_sources(unit_tests PRIVATE
    flt.cpp
    log2.cpp
    xor_msb.cpp
    less.cpp
)

target_link_libraries(unit_tests
    GTest::gtest GTest::gtest_main zorder_knn
)

gtest_discover_tests(unit_tests)
