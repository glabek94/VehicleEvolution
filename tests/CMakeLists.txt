cmake_minimum_required(VERSION 3.5)
project(VehicleTest)

enable_testing()

# indicates the location of the boost installation tree.
# hard-coded for our simple example.
set(BOOST_INCLUDE_DIRS $boost_installation_prefix/include)

# creates the executable
add_executable(vehicle_tests VehicleTests.cpp)

# indicates the include paths
target_include_directories(vehicle_tests PRIVATE ${BOOST_INCLUDE_DIRS})
# declares a test with our executable
add_test(NAME VehicleTests COMMAND vehicle_tests)