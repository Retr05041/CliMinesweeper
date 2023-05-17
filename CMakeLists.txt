# CMakeLists.txt

# minimum version of CMake required to build this project
cmake_minimum_required(VERSION 3.22.1)

# Project name
# Current version
# Languages used
project(Minesweeper VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-std=c++17)

# Set source files
set(SOURCE_FILES
    src/main.cpp
    src/board/Board.cpp
    src/tile/Tile.cpp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})