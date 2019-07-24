cmake_minimum_required(VERSION 3.9.5)
project(CG)

set(CMAKE_CXX_STANDARD 17)

add_executable(CG
                main.cpp
                Sphere.cpp
                Bmp.cpp
                Sphere.h
                Bmp.h
                bin/libIrrKlang.so
                include/irrKlang.h
            )


target_link_libraries(CG -lGL -lGLU -lglut -pthread ${CMAKE_SOURCE_DIR}/bin/libIrrKlang.so)
