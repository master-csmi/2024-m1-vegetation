cmake_minimum_required(VERSION 3.22)
# To compile use : cmake --preset debug /path_to_CMakeLists.txt

# project name
project(vegetation CXX)

if( NOT CMAKE_BUILD_TYPE )
    set(CMAKE_BUILD_TYPE Release) 
endif()

find_package(CGAL REQUIRED)

# where to get the headers files
include_directories(include)

# making the list of sources files
file(GLOB_RECURSE SOURCE_FILES src/*)

# creating the library with the sources files
add_library(${PROJECT_NAME} ${SOURCE_FILES})

# creating the executable from src/main.cpp
add_executable(run src/main.cpp)

target_link_libraries(run CGAL::CGAL ${PROJECT_NAME})  # Link with CGAL and vegetation library

# Unitary testing
include(CTest)
enable_testing()
message(STATUS "Adding tests : ")
# getting test files
file(GLOB_RECURSE TEST_FILES test/*.cpp)
foreach(TEST_FILE ${TEST_FILES})
    # get the name without the extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    # creating the executable
    add_executable(${TEST_NAME}.out ${TEST_FILE})
    # linking the executable with the library
    target_link_libraries(${TEST_NAME}.out ${PROJECT_NAME})
    # adding the test_name which uses test_name.out executable
    add_test(${TEST_NAME} ${TEST_NAME}.out)
    message("       - ${TEST_NAME}")        
endforeach()
