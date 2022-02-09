cmake_minimum_required(VERSION 2.8.12)
project(CppPractice)

add_definitions("-std=c++17")
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(cpppractice  src/main.cpp)
target_link_libraries(cpppractice ${CONAN_LIBS})