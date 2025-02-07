cmake_minimum_required(VERSION 3.20)
project(client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

file(GLOB_RECURSE SOURCES src/*.cpp src/**/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp include/**/*.hpp)

add_executable(Client ${SOURCES} ${HEADERS})

target_link_libraries(Client sfml-graphics sfml-window sfml-system sfml-audio GameEngine)
target_include_directories(Client PUBLIC include ../external/smart-buffer-cpp/include)