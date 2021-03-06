cmake_minimum_required(VERSION 3.5)

project(VehicleEvolution)

set(CMAKE_CXX_STANDARD 14)

include_directories(../include)

find_library(LIBBOX2D Box2D ../lib)
find_library(LIBSFML-GRAPHICS sfml-graphics ../lib)
find_library(LIBSFML-WINDOW sfml-window ../lib)
find_library(LIBSFML-SYSTEM sfml-system ../lib)

add_executable(VehicleEvolution main.cpp Vehicle.cpp ../include/Vehicle.h
        Application.cpp ../include/Application.h Chromosome.cpp ../include/Chromosome.h
        EvolutionaryAlgorithm.cpp ../include/EvolutionaryAlgorithm.h
        ../include/RandomNumberGenerator.h GroundFactory.cpp ../include/GroundFactory.h
        GroundChain.cpp ../include/GroundChain.h ../include/Constants.h
        ../include/World.h World.cpp)
target_link_libraries(VehicleEvolution ${LIBBOX2D})
target_link_libraries(VehicleEvolution ${LIBSFML-GRAPHICS})
target_link_libraries(VehicleEvolution ${LIBSFML-WINDOW})
target_link_libraries(VehicleEvolution ${LIBSFML-SYSTEM})

file(COPY ../res/arial.ttf
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
