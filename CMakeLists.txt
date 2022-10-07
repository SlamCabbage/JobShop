cmake_minimum_required(VERSION 3.10)
project(jobshop)

set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

aux_source_directory(. DIR_SRCS)
add_subdirectory(include)
add_executable(jobshop ${DIR_SRCS})

target_link_libraries(jobshop buffer)
