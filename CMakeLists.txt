cmake_minimum_required(VERSION 2.8.12)
project(CppPractice)

add_definitions("-std=c++17")
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_compile_options(-fsanitize=address)
add_compile_options(-fsanitize=thread)
# add_link_options(-fsanitize=address)
add_link_options(-fsanitize=thread)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.h)

add_executable(cpppractice ${SOURCE_FILES})
target_compile_options(cpppractice PRIVATE --save-temps)
target_link_libraries(cpppractice ${CONAN_LIBS})