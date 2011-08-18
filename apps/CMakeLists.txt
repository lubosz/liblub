cmake_minimum_required(VERSION 2.8)
project(liblub)

#build examples
file(GLOB_RECURSE EXAMPLES *.cpp)

foreach(EXAMPLE_PATH ${EXAMPLES})
    STRING(REGEX REPLACE "\\.cpp" "" EXAMPLE_NAME "${EXAMPLE_PATH}")
    STRING(REGEX REPLACE "^.*/([^/]*)\$" "\\1" EXAMPLE_NAME "${EXAMPLE_NAME}")
    add_executable(${EXAMPLE_NAME} "${EXAMPLE_PATH}")
    target_link_libraries(${EXAMPLE_NAME} lub)
endforeach(EXAMPLE_PATH)
