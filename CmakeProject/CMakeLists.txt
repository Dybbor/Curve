cmake_minimum_required(VERSION 3.5)
project(TestTask LANGUAGES CXX)

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set (SRC Source.cpp Figure.cpp Circle.cpp Ellipse.cpp Helix.cpp)
set (HEADERS Figure.h Circle.h Ellipse.h Helix.h)

add_executable(TestTask ${SRC} ${HEADERS})

if(OpenMP_CXX_FOUND)
    target_link_libraries(TestTask PUBLIC OpenMP::OpenMP_CXX)
endif()