cmake_minimum_required(VERSION 2.6)
project(liblub)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

#Optimization / Debug
set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
#set(CMAKE_CXX_FLAGS "-O3") #Optimized

if (UNIX)
#Hack For SDL 1.3 in local
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib -I/usr/local/include/SDL -D_REENTRANT -Wl,-rpath,/usr/local/lib")

#Set c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif (UNIX)
#MESSAGE("Flags: ${CMAKE_CXX_FLAGS}")


include_directories(
include
#include/GL3
)

#App Shader Demo
set(SOURCES
	${SRC_DIR}/main.cpp
	${SRC_DIR}/MediaLayer.cpp 
	${SRC_DIR}/RenderEngine.cpp  	
	
	${SRC_DIR}/Scene/Camera.cpp
	${SRC_DIR}/Scene/SceneGraph.cpp
	${SRC_DIR}/Scene/Node.cpp
	#${SRC_DIR}/Material.cpp
 
	${SRC_DIR}/Material/ShaderProgram.cpp
	${SRC_DIR}/Material/Shader.cpp
	${SRC_DIR}/Material/Texture.cpp
	${SRC_DIR}/Material/TextureFactory.cpp
	#${SRC_DIR}/Material/ShaderFactory.cpp

	${SRC_DIR}/Mesh/ObjLoader.cpp
	${SRC_DIR}/Mesh/Geometry.cpp  
	${SRC_DIR}/Mesh/Mesh.cpp
	${SRC_DIR}/Mesh/MeshFactory.cpp
	${SRC_DIR}/Mesh/Vector2D.cpp
	${SRC_DIR}/Mesh/Vector3D.cpp
	${SRC_DIR}/Mesh/Face.cpp
	${SRC_DIR}/Mesh/MengerSponge.cpp
)

add_executable(liblub ${SOURCES})
target_link_libraries(liblub GL SDL freeimageplus)
