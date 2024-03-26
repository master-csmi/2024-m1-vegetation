cmake_minimum_required(VERSION 3.27)
# To compile use : cmake --preset debug /path_to_CMakeLists.txt

# project name
project(vegetation)

# Enable generation of compile_commands.json
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
 
# where do get the headers files
include_directories(include)

# making the list of sources files
file(GLOB_RECURSE SOURCE_FILES src/*)

# creating the library with the sources files
add_library(${PROJECT_NAME} ${SOURCE_FILES})

# creating the executable from src/main.cpp
add_executable(run src/main.cpp)

# linking the executable with the library
target_link_libraries(run ${PROJECT_NAME})

# linkind the executable with the gmp and mpfr libraries for CGAL
target_link_libraries(run gmp mpfr)



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



  




