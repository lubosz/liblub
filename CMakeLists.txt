cmake_minimum_required(VERSION 2.6)
project(liblub)

#Set Options
OPTION(WITH_SDL "Use SDL" OFF)
OPTION(SDL_LOCAL "Use SDL locally" OFF)
OPTION(RELEASE "Release" OFF)

FIND_PACKAGE(Qt4 REQUIRED)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(APP_DIR ${CMAKE_SOURCE_DIR}/examples)

if(SDL_LOCAL)
    set(SDL_DIR /home/bmonkey/Apps/Graphics/SDL/)
ENDIF()

#Optimization / Debug
IF(RELEASE)
    set(CMAKE_CXX_FLAGS "-O3 -msse4 -mssse3") #Optimized
ELSE()
    set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
ENDIF()

if (UNIX)
#Hack For SDL 1.3 in local
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib -L/usr/local/Trolltech/Qt-4.7.0/lib -I/usr/local/include/SDL -D_REENTRANT -Wl,-rpath,/usr/local/lib")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/home/bmonkey/Apps/Graphics/SDL/build/.libs")


#Set c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif (UNIX)
#MESSAGE("Flags: ${CMAKE_CXX_FLAGS}")

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
#SET(QT_USE_QTOPENGL TRUE)
SET(QT_USE_QTMAIN TRUE)

include_directories(
	include
	include/common
	${QT_INCLUDES}	
	if(SDL_LOCAL)
	    ${SDL_DIR}include
	else()
	   /usr/include/SDL/
	endif()
#include/GL3
)

#App Shader Demo
set(SOURCES

	${SRC_DIR}/Scene/Camera.cpp
	${SRC_DIR}/Scene/SceneGraph.cpp
	${SRC_DIR}/Scene/DirectionNode.cpp
	
	${SRC_DIR}/Renderer/OpenGL.cpp
	${SRC_DIR}/Renderer/FrameBuffer.cpp
	${SRC_DIR}/Renderer/RenderPass.cpp
	${SRC_DIR}/Renderer/RenderPasses.cpp
	${SRC_DIR}/Renderer/RenderSequence.cpp
 
	${SRC_DIR}/Material/ShaderProgram.cpp
	${SRC_DIR}/Material/Shader.cpp
	${SRC_DIR}/Material/Texture.cpp
	${SRC_DIR}/Material/Textures.cpp
	${SRC_DIR}/Material/TextureFactory.cpp
	${SRC_DIR}/Material/Material.cpp

	${SRC_DIR}/Mesh/Geometry.cpp  
	${SRC_DIR}/Mesh/Mesh.cpp
	${SRC_DIR}/Mesh/MeshFactory.cpp
	${SRC_DIR}/Mesh/MengerSponge.cpp
	
	${SRC_DIR}/Scene/Node.cpp
	${SRC_DIR}/Scene/Light.cpp
)

IF(WITH_SDL)
	ADD_DEFINITIONS(-DWITH_SDL)
	set(SOURCES ${SOURCES} ${SRC_DIR}/Window/SDL.cpp)
	set(SOURCES ${SOURCES} ${SRC_DIR}/Window/Input/SDL-loop.cpp)
ELSE()
    ADD_DEFINITIONS(-DWITH_XCB)
	set(SOURCES ${SOURCES} ${SRC_DIR}/Window/XCB.cpp)
	#set(SOURCES ${SOURCES} ${SRC_DIR}/Window/Input/XCB-events.cpp)
	set(SOURCES ${SOURCES} ${SRC_DIR}/Window/Input/XCB-loop.cpp)
ENDIF()
	 
add_library(lub ${SOURCES})
target_link_libraries(lub GL freeimageplus ${QT_LIBRARIES} assimp)

IF(WITH_SDL)
    if(SDL_LOCAL)
	    target_link_libraries(lub ${SDL_DIR}build/.libs/libSDL.a)
	else()
		target_link_libraries(lub SDL)
	endif()
ELSE()
	target_link_libraries(lub X11-xcb xcb-keysyms)
ENDIF()

set(EXAMPLES
    stars
    planets
    buffer
    nice
    sponge
    waterplanet
    csm
    spiral
    shadow
)

foreach(example ${EXAMPLES})
  add_executable(${example} ${APP_DIR}/${example}.cpp)
  target_link_libraries(${example} lub)
endforeach(example)
