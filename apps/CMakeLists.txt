cmake_minimum_required(VERSION 2.8)
project(liblub)

find_package(Qt4 REQUIRED)

#build examples
file(GLOB EXAMPLES *.cpp)

include_directories(
    ../demos
)

set(SOURCES ${SOURCES} ${LIBLUB_MOC_OUTFILES})

foreach(EXAMPLE_PATH ${EXAMPLES})
    STRING(REGEX REPLACE "\\.cpp" "" EXAMPLE_NAME "${EXAMPLE_PATH}")
    STRING(REGEX REPLACE "^.*/([^/]*)\$" "\\1" EXAMPLE_NAME "${EXAMPLE_NAME}")
    
    set(EXAMPLE_SOURCES ${EXAMPLE_PATH})
    
    add_executable("${EXAMPLE_NAME}" ${EXAMPLE_SOURCES})
    target_link_libraries(${EXAMPLE_NAME} lub lubDemos)
    install(TARGETS ${EXAMPLE_NAME} DESTINATION "${CMAKE_INSTALL_BINDIR}")
endforeach(EXAMPLE_PATH)

ADD_SUBDIRECTORY(editor)
ADD_SUBDIRECTORY(planet-editor)
