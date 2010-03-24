cmake_minimum_required(VERSION 2.6)
project(gl3Cone)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

#set(CMAKE_CXX_FLAGS "-O0 -g3")
#set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS "-std=gnu++0x -O0 -g3 -L/usr/local/lib -I/usr/local/include/SDL -D_REENTRANT -Wl,-rpath,/usr/local/lib")

include_directories(include include/tnt)
#App Shader Demo
set(CONE_DEMO_SOURCES
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/utils.cpp
  ${SRC_DIR}/MediaLayer.cpp
  ${SRC_DIR}/ShaderProgram.cpp
  ${SRC_DIR}/Shader.cpp
  ${SRC_DIR}/RenderEngine.cpp
  ${SRC_DIR}/Mesh.cpp
)

add_executable(gl3Cone ${CONE_DEMO_SOURCES})
target_link_libraries(gl3Cone GL  SDL freeimageplus)
