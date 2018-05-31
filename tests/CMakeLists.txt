cmake_minimum_required(VERSION 3.5)
project(VehicleTest)

set(CMAKE_CXX_STANDARD 11)
#boost
enable_testing()
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB VehicleEvolutionSrc ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM VehicleEvolutionSrc "${CMAKE_SOURCE_DIR}/src/main.cpp")

find_library(LIBBOX2D Box2D ../lib)
find_library(LIBSFML-GRAPHICS sfml-graphics ../lib)
find_library(LIBSFML-WINDOW sfml-window ../lib)
find_library(LIBSFML-SYSTEM sfml-system ../lib)

#find_library(LIBBOX2D Box2D ${CMAKE_SOURCE_DIR}/lib)
#find_library(LIBSFML-GRAPHICS NAMES sfml-graphics PATHS "${CMAKE_SOURCE_DIR}/lib/libsfml-graphics.so" NO_DEFAULT_PATH)
#find_library(LIBSFML-WINDOW sfml-window PATHS ${CMAKE_SOURCE_DIR}/lib NO_CMAKE_SYSTEM_PATH)
#find_library(LIBSFML-SYSTEM sfml-system PATHS ${CMAKE_SOURCE_DIR}/lib NO_CMAKE_SYSTEM_PATH)


# creates the executable
add_executable(vehicle_tests VehicleTests.cpp ${VehicleEvolutionSrc})


target_include_directories(vehicle_tests PRIVATE ${Boost_INCLUDE_DIRS} PUBLIC ../include/)

target_link_libraries(VehicleEvolution ${LIBBOX2D})
target_link_libraries(VehicleEvolution ${LIBSFML-GRAPHICS})
target_link_libraries(VehicleEvolution ${LIBSFML-WINDOW})
target_link_libraries(VehicleEvolution ${LIBSFML-SYSTEM})

#target_link_libraries(vehicle_tests PUBLIC "${LIBBOX2D}")
#target_link_libraries(vehicle_tests PUBLIC "${CMAKE_SOURCE_DIR}/lib/libsfml-graphics.so")
#target_link_libraries(vehicle_tests PUBLIC "${CMAKE_SOURCE_DIR}/lib/libsfml-window.so")
#target_link_libraries(vehicle_tests PUBLIC "${CMAKE_SOURCE_DIR}/lib/libsfml-system.so")

# declares a test with our executable
add_test(NAME VehicleTests COMMAND vehicle_tests)