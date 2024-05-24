# Add the test executable
add_executable(
    tests
    test_config.cpp
    test_tree.cpp
    test_query.cpp
    test_union.cpp
)

# Link the test executable with the Tree library and Catch2
target_link_libraries(tests PRIVATE Tree Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)
