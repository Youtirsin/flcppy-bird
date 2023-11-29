cmake_minimum_required(VERSION 3.0.0)
project(flappy-bird VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

include_directories("include")

add_executable(flappy-bird main.cpp)
target_link_libraries(flappy-bird PRIVATE sfml-system sfml-graphics sfml-window sfml-audio)