# Minimum required CMake version
cmake_minimum_required(VERSION 3.15)

# Project name
project(Vegetation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Set build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Include FetchContent module
include(FetchContent)

# Fetch Catch2 library
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)

# Include src and test directories
add_subdirectory(src)
add_subdirectory(test)
