cmake_minimum_required(VERSION 2.8)
project(liblub)

#build tests

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/test)

#find_package(CPPUnit REQUIRED)
#find_library(CPPUNIT_LIBRARY 
#            NAMES cppunit
#            PATHS   /usr/lib
#                    /usr/lib64
#                    /usr/local/lib
#                    /usr/local/lib64)

if(BUILD_TESTS)

    add_subdirectory(eglut)

    file(GLOB TESTS ${TEST_DIR}/*.cpp)
    foreach(TEST_PATH ${TESTS})
        string(REGEX REPLACE "\\.cpp" "" TEST_NAME "${TEST_PATH}")
        string(REGEX REPLACE "^.*/([^/]*)\$" "\\1" TEST_NAME "${TEST_NAME}")
        add_executable(${TEST_NAME} "${TEST_PATH}")
        target_link_libraries(${TEST_NAME} lub lubApp cppunit)
    endforeach(TEST_PATH)

target_link_libraries(backend-wayland EGL wayland-client wayland-egl)

endif()

