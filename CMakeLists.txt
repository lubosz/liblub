cmake_minimum_required(VERSION 2.6)
project(liblub)

FIND_PACKAGE(Qt4 REQUIRED)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(APP_DIR ${CMAKE_SOURCE_DIR}/examples)

#Optimization / Debug
set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
#set(CMAKE_CXX_FLAGS "-O3 -msse4 -mssse3") #Optimized

if (UNIX)
#Hack For SDL 1.3 in local
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib -I/usr/local/include/SDL -D_REENTRANT -Wl,-rpath,/usr/local/lib")

#Set c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif (UNIX)
#MESSAGE("Flags: ${CMAKE_CXX_FLAGS}")

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})


include_directories(
include
#include/GL3
)

#App Shader Demo
set(SOURCES

	${SRC_DIR}/Scene/Camera.cpp
	${SRC_DIR}/Scene/SceneGraph.cpp

	#${SRC_DIR}/Scene/Matrix.cpp


	
	${SRC_DIR}/Renderer/OpenGL.cpp
	${SRC_DIR}/Renderer/FrameBuffer.cpp
	${SRC_DIR}/Renderer/RenderPass.cpp
	

 
	${SRC_DIR}/Material/ShaderProgram.cpp
	${SRC_DIR}/Material/Shader.cpp
	${SRC_DIR}/Material/Texture.cpp
	${SRC_DIR}/Material/TextureFactory.cpp
	${SRC_DIR}/Material/Material.cpp

	${SRC_DIR}/Mesh/ObjLoader.cpp
	${SRC_DIR}/Mesh/Geometry.cpp  
	${SRC_DIR}/Mesh/Mesh.cpp
	${SRC_DIR}/Mesh/MeshFactory.cpp
	${SRC_DIR}/Mesh/Vector2D.cpp
	${SRC_DIR}/Mesh/Vector3D.cpp
	${SRC_DIR}/Mesh/Face.cpp
	${SRC_DIR}/Mesh/MengerSponge.cpp
	
	${SRC_DIR}/Scene/Node.cpp
	
	${SRC_DIR}/Window/SDL.cpp 
)

add_library(lub ${SOURCES})
target_link_libraries(lub GL SDL freeimageplus QtGui)

add_executable(planets ${APP_DIR}/planets.cpp)
target_link_libraries(planets lub)
add_executable(buffer ${APP_DIR}/buffer.cpp)
target_link_libraries(buffer lub)
add_executable(nice ${APP_DIR}/nice.cpp)
target_link_libraries(nice lub)
add_executable(sponge ${APP_DIR}/sponge.cpp)
target_link_libraries(sponge lub)
