cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES src/*.cpp src/**/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp include/**/*.hpp)

add_executable(Server ${SOURCES} ${HEADERS})

target_include_directories(Server PUBLIC include)