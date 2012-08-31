cmake_minimum_required(VERSION 2.8)
project(liblub)

find_package(Qt4 REQUIRED)

#build examples
file(GLOB DEMOS *.cpp)

add_library(lubDemos ${DEMOS})
target_link_libraries(lubDemos lub)
