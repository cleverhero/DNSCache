cmake_minimum_required(VERSION 3.16)
project(HWMX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/)

enable_testing()

add_subdirectory(lib)
add_subdirectory(tests)

add_executable(example1 examples/example1.cpp)
target_include_directories(example1 PUBLIC ${HEADERS_DIR})
target_link_libraries(example1 dnsc_lib)
