cmake_minimum_required(VERSION 3.22)
# To compile use : cmake --preset debug /path_to_CMakeLists.txt

# project name
project(vegetation)

if( NOT CMAKE_BUILD_TYPE )
        set(CMAKE_BUILD_TYPE Release) 
endif()

find_package(CGAL REQUIRED)

# Enable generation of compile_commands.json
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
 
# where do get the headers files
include_directories(include)

# making the list of sources files
file(GLOB_RECURSE SOURCE_FILES src/*)

# creating the executable from src/main.cpp
add_executable(run src/main.cpp)
target_link_libraries(run CGAL::CGAL)
# linking the executable with the library

# Unitary testing
enable_testing()
message(STATUS "Adding tests : ")
# getting test files
file(GLOB_RECURSE TEST_FILES tests/*.cpp)
foreach(TEST_FILES ${TEST_FILES})
        # get the name without the extension
        get_filename_component(TEST_NAME ${TEST_FILES} NAME_WE)
        # creating the executable
        add_executable(${TEST_NAME}.out ${TEST_FILES})
        # linking the executable with the library
        target_link_libraries(${TEST_NAME}.out ${PROJECT_NAME})
        # addind the test_name which uses test_name.out executable
        add_test(${TEST_NAME} ${TEST_NAME}.out)
        message("       - ${TEST_NAME}")        
endforeach()

