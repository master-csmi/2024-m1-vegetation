# Set a variable for all the libraries that are required
set(requiredlibs)

# Find libraries
find_package(CGAL REQUIRED COMPONENTS Core)
if(CGAL_FOUND)
    set(requiredlibs ${requiredlibs} CGAL::CGAL CGAL::CGAL_Core ${CGAL_3RD_PARTY_LIBRARIES})
else()
    message(STATUS "This program requires the CGAL library, and will not be compiled.")
    return()
endif()

find_package(CURL REQUIRED)
if(CURL_FOUND)
    set(requiredlibs ${requiredlibs} CURL::libcurl)
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

include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
file(GLOB_RECURSE SOURCES_FILES *.cpp)

# Exclude specific main source files from the Tree library source files
list(REMOVE_ITEM SOURCES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/wrapper.cpp ${CMAKE_CURRENT_SOURCE_DIR}/simplify.cpp)

# Create library
add_library(Tree ${SOURCES_FILES})

# Create main executables
add_executable(run main.cpp)
add_executable(wrap wrapper.cpp)
add_executable(simplify simplify.cpp)

# Link libraries
target_link_libraries(Tree PUBLIC ${requiredlibs})
target_include_directories(Tree PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Link main executables to the library and required libraries
target_link_libraries(run PRIVATE Tree ${requiredlibs})
target_link_libraries(wrap PRIVATE Tree ${requiredlibs})
target_link_libraries(simplify PRIVATE Tree ${requiredlibs})
