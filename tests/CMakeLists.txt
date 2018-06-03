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

# creates the executable
add_executable(vehicle_tests VehicleTests.cpp ${VehicleEvolutionSrc})

# for code coverage
SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS "-lgcov")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

target_include_directories(vehicle_tests PRIVATE ${Boost_INCLUDE_DIRS} PUBLIC ../include/)

target_link_libraries(vehicle_tests ${LIBBOX2D})
target_link_libraries(vehicle_tests ${LIBSFML-GRAPHICS})
target_link_libraries(vehicle_tests ${LIBSFML-WINDOW})
target_link_libraries(vehicle_tests ${LIBSFML-SYSTEM})

# declares a test with our executable
add_test(NAME VehicleTests COMMAND vehicle_tests)