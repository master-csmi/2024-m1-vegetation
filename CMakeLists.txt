# Minimum required CMake version
cmake_minimum_required(VERSION 3.15)

# Project name
project(Vegetation CXX)

set(requiredlibs)

# Find libraries
find_package(CGAL REQUIRED)
if(CGAL AND CGAL_Qt5_FOUND)
    add_definitions(-DCGAL_USE_BASIC_VIEWER)
    set(requiredlibs ${requiredlibs} ${CGAL_3RD_PARTY_LIBRARIES})
endif()

find_package(CURL REQUIRED)
if(CURL_FOUND)
    set(requiredlibs ${requiredlibs} ${CURL_LIBRARIES})
endif()


# Source files
file(GLOB_RECURSE SOURCES_FILES src/*.cpp)

# Set build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

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
add_library(Vegetation ${SOURCES_FILES})

# Create executable
add_executable(run src/main.cpp)

# Link the executable with the curl library
target_link_libraries(Vegetation PUBLIC ${requiredlibs})

target_include_directories(Vegetation PUBLIC include)

# Link libraries
target_link_libraries(run PRIVATE Vegetation ${requiredlibs})

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
    target_link_libraries(${test_name} Vegetation) # Fixed library name
endforeach()