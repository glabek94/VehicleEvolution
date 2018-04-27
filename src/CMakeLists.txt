cmake_minimum_required(VERSION 3.9)

project(VehicleEvolution)

set(CMAKE_CXX_STANDARD 14)

include_directories(../include)

find_library(LIBBOX2D Box2D lib)
find_library(LIBSFML-GRAPHICS sfml-graphics lib)
find_library(LIBSFML-WINDOW sfml-window lib)
find_library(LIBSFML-SYSTEM sfml-system lib)

add_executable(VehicleEvolution main.cpp Vehicle.cpp ../include/Vehicle.h Application.cpp ../include/Application.h)
target_link_libraries(VehicleEvolution ${LIBBOX2D})
target_link_libraries(VehicleEvolution ${LIBSFML-GRAPHICS})
target_link_libraries(VehicleEvolution ${LIBSFML-WINDOW})
target_link_libraries(VehicleEvolution ${LIBSFML-SYSTEM})