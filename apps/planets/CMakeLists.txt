cmake_minimum_required(VERSION 2.8)
project(liblub)

file(GLOB_RECURSE CPP_FILES *.cpp)

add_executable(planets ${CPP_FILES})
target_link_libraries(planets lub)
