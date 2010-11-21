cmake_minimum_required(VERSION 2.6)
project(liblub)

#Set Options
OPTION(WITH_SDL "Use SDL" OFF)
OPTION(SDL_LOCAL "SDL Local" OFF)
OPTION(RELEASE "Release" OFF)
#SET(MYVAR "Test" CACHE STRING "This is my variable")


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

#Hack For SDL 1.3 in local
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/home/bmonkey/Apps/Graphics/SDL/build/.libs")


#Set c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

INCLUDE(${QT_USE_FILE})

function(download_gl_header URL)
		execute_process(
			WORKING_DIRECTORY include/GL3
			COMMAND wget ${URL}
		)
endfunction(download_gl_header)

IF(EXISTS /usr/include/GL3)
	message("OpenGL 3 Headers in /usr/include")
ELSE()
	message("No OpenGL 3 Headers found in /usr! Using local ones")
	IF(NOT EXISTS include/GL3)
		execute_process(COMMAND mkdir include/GL3)
	ENDIF()
	IF(NOT EXISTS include/GL3/gl3.h)
		download_gl_header(http://www.opengl.org/registry/api/gl3.h)
	ENDIF()
	IF(NOT EXISTS include/GL3/glext.h)
		download_gl_header(http://www.opengl.org/registry/api/glext.h)
	ENDIF()
	IF(NOT EXISTS include/GL3/glxext.h)
		download_gl_header(http://www.opengl.org/registry/api/glxext.h)
	ENDIF()

	include_directories(include/GL3)
ENDIF()


#Check for nvidia libGL in ubuntu
IF(EXISTS "/usr/lib/nvidia-current/libGL.so")
	message("Taking Ubuntu Nvidia libGL")
	SET(OPENGL_LIB /usr/lib/nvidia-current/libGL.so)
ELSE()
	message("Taking system libGL")
	SET(OPENGL_LIB GL)
ENDIF() 

include_directories(
	include
	include/common
	${QT_INCLUDES}	
	if(SDL_LOCAL)
	    ${SDL_DIR}include
	else()
	   /usr/include/SDL/
	endif()
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
	set(SOURCES ${SOURCES} ${SRC_DIR}/Window/Input/XCB-loop.cpp)
ENDIF()
	 
add_library(lub ${SOURCES})
target_link_libraries(lub ${OPENGL_LIB} freeimageplus ${QT_LIBRARIES} assimp)

IF(WITH_SDL)
    if(SDL_LOCAL)
	    target_link_libraries(lub ${SDL_DIR}build/.libs/libSDL.a)
	else()
		target_link_libraries(lub SDL)
	endif()
ELSE()
	target_link_libraries(lub X11-xcb xcb-keysyms)
ENDIF()

file(GLOB_RECURSE EXAMPLES ${APP_DIR}/*.cpp)

foreach(EXAMPLE_PATH ${EXAMPLES})
    STRING(REGEX REPLACE "\\.cpp" "" EXAMPLE_NAME "${EXAMPLE_PATH}")
    STRING(REGEX REPLACE "^.*/([^/]*)\$" "\\1" EXAMPLE_NAME "${EXAMPLE_NAME}")
    add_executable(${EXAMPLE_NAME} "${EXAMPLE_PATH}")
    target_link_libraries(${EXAMPLE_NAME} lub)
endforeach(EXAMPLE_PATH)
