cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(HAVE_CXX14 TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(computelibrary)

set(COMPUTELIB_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(utils)
