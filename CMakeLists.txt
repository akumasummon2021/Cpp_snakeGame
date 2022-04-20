cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall -rdynamic -ldl")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(SDL2_DIR "C:/Works/Git/CppSnake/SDL2")
include_directories("C:/Works/Git/CppSnake/SDL2/include")


find_package(SDL2 REQUIRED)


add_executable(SnakeGame src/main.cpp src/food src/game.cpp src/stone.cpp src/controller.cpp src/renderer.cpp src/snake.cpp src/Astar.cpp)
# string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(SnakeGame ${SDL2_LIBRARIES})

