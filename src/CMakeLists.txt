add_executable(wrap main.cpp)
target_link_libraries(wrap PRIVATE project_options 
CONAN_PKG::eigen
CONAN_PKG::cgal)
