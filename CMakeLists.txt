# Minimum required CMake version
cmake_minimum_required(VERSION 3.15)

# Project name
project(Vegetation CXX)

# Enable testing
enable_testing()

# Set build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Include src and test directories
add_subdirectory(src)
add_subdirectory(test)
