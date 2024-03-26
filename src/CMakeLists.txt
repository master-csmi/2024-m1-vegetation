add_executable(run main.cpp)
target_link_libraries(run PRIVATE project_options 
CONAN_PKG::eigen
CONAN_PKG::cgal)
