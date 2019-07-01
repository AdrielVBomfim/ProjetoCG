cmake_minimum_required(VERSION 3.14)
project(CG)

set(CMAKE_CXX_STANDARD 17)

add_executable(CG projeto.cpp)
target_link_libraries(CG -lglut -lGL -lGLU)