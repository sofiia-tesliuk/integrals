cmake_minimum_required(VERSION 3.13)
project(integrals)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES config.cpp integral.cpp)
set(CMAKE_CXX_FLAGS "-pthread -O3")
add_executable(consistent ${SOURCE_FILES} consistent.cpp)
add_executable(parallel ${SOURCE_FILES} parallel.cpp)

find_package(Threads REQUIRED)
target_link_libraries(parallel Threads::Threads)