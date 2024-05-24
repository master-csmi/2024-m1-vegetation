find_package(Catch2 3 REQUIRED)

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

include(CTest)
include(Catch)
catch_discover_tests(tests)
