cmake_minimum_required(VERSION 2.6)
project(liblub)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

#Optimization / Debug
#set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
set(CMAKE_CXX_FLAGS "-O3") #Optimized

#Hack For SDL 1.3 in local
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib -I/usr/local/include/SDL -D_REENTRANT -Wl,-rpath,/usr/local/lib")

#Set c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

#MESSAGE("Flags: ${CMAKE_CXX_FLAGS}")


include_directories(
include
include/tnt
)

#App Shader Demo
set(CONE_DEMO_SOURCES
	${SRC_DIR}/Camera.cpp
	#${SRC_DIR}/Material.cpp
	${SRC_DIR}/RenderEngine.cpp   
	${SRC_DIR}/ShaderProgram.cpp   
	${SRC_DIR}/ObjLoader.cpp
	${SRC_DIR}/Geometry.cpp  
	${SRC_DIR}/MediaLayer.cpp  
	${SRC_DIR}/Shader.cpp
	${SRC_DIR}/main.cpp
	${SRC_DIR}/Texture.cpp
	${SRC_DIR}/Mesh.cpp
	${SRC_DIR}/TextureFactory.cpp
	${SRC_DIR}/ShaderFactory.cpp
	${SRC_DIR}/SceneGraph.cpp
)

add_executable(liblub ${CONE_DEMO_SOURCES})
target_link_libraries(liblub GL GLU SDL freeimageplus)
