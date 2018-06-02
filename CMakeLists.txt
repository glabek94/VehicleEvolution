cmake_minimum_required(VERSION 3.5)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W2 /O2")
endif()


add_subdirectory(src)
add_subdirectory(tests)