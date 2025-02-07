cmake_minimum_required(VERSION 3.20)
project(RType)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/SFML)
add_subdirectory(external/smart-buffer-cpp)
add_subdirectory(game_engine)
add_subdirectory(server)
add_subdirectory(client)