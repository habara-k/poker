cmake_minimum_required(VERSION 3.9)
project(poker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")

set(SOURCE_FILES main.cpp)
add_executable(${CMAKE_PROJECT_NAME}.out ${SOURCE_FILES})

include_directories(lib)

add_subdirectory(lib)
add_subdirectory(test)

target_link_libraries(${CMAKE_PROJECT_NAME}.out ${CMAKE_PROJECT_NAME})
