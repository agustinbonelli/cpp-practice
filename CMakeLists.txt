cmake_minimum_required(VERSION 2.8.12)
project(CppPractice)

add_definitions("-std=c++17")
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)

# add_compile_options(-fsanitize=address)
add_compile_options(-fsanitize=thread)
# add_link_options(-fsanitize=address)
add_link_options(-fsanitize=thread)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(cpppractice  
    src/main.cpp
    src/exceptions_in_destructors.cpp
    src/interlace.cpp
    src/spiral_iterator.cpp
    src/shared_ptr/shared_ptr_test.cpp
)
target_compile_options(cpppractice PRIVATE --save-temps)
target_link_libraries(cpppractice ${CONAN_LIBS})