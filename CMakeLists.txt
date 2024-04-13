# Minimum required CMake version
cmake_minimum_required(VERSION 3.15)

# Project name
project(Vegetation CXX)

# Set build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set a variable for all the libraries that are required
set(requiredlibs)

# Find libraries
find_package(CGAL REQUIRED)
if(CGAL_FOUND)
    set(requiredlibs ${requiredlibs} ${CGAL_3RD_PARTY_LIBRARIES})
else()
    message(STATUS "This program requires the CGAL library, and will not be compiled.")
    return()
endif()

find_package(CURL REQUIRED)
if(CURL_FOUND)
    set(requiredlibs ${requiredlibs} ${CURL_LIBRARIES})
else()
    message(STATUS "This program requires the CURL library, and will not be compiled.")
    return()
endif()

find_package(GMP REQUIRED)
if(GMP_FOUND)
    set(requiredlibs ${requiredlibs} ${GMP_LIBRARIES})
else()
    message(STATUS "This program requires the GMP library, and will not be compiled.")
    return()
endif()

find_package(MPFR REQUIRED)
if(MPFR_FOUND)
    set(requiredlibs ${requiredlibs} ${MPFR_LIBRARIES})
else()
    message(STATUS "This program requires the MPFR library, and will not be compiled.")
    return()
endif()


# Source files
file(GLOB_RECURSE SOURCES_FILES src/*.cpp)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -fno-sanitize=vptr")

# Output build type
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message("   Compiler CXX:         ${CMAKE_CXX_COMPILER}")
message("   Compiler CXX flags:   ${CMAKE_CXX_FLAGS}")

# Output debug flags if in DEBUG mode
if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
    message("   Compiler CXX debug flags:   ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

# Output release flags if in RELEASE mode
if(CMAKE_BUILD_TYPE MATCHES "RELEASE")
    message("   Compiler CXX release flags: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

# Create library
add_library(Tree ${SOURCES_FILES})

# Create executable
add_executable(run src/main.cpp)

# Link the executable with the curl library
target_link_libraries(Tree PUBLIC ${requiredlibs})

target_include_directories(Tree PUBLIC include)

# Link libraries
target_link_libraries(run PRIVATE Tree ${requiredlibs})

# Testing
include(CTest)
enable_testing()

# Test files
file(GLOB_RECURSE TESTS_FILES test/*.cpp)

# Add tests
foreach(test_full_path ${TESTS_FILES})
    get_filename_component(test_filename ${test_full_path} NAME)
    string(CONCAT test_filename "test/" ${test_filename})
    get_filename_component(test_name ${test_filename} NAME_WE)
    add_executable(${test_name} ${test_filename})
    add_test(${test_name} ${test_name})
    target_link_libraries(${test_name} Tree) # Fixed library name
endforeach()