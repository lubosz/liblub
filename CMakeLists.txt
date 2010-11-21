cmake_minimum_required(VERSION 2.8)
project(liblub)

#Set Options
OPTION(WITH_SDL "Use SDL" OFF)
OPTION(SDL_LOCAL "SDL Local" OFF)
OPTION(RELEASE "Release" OFF)
#SET(MYVAR "Test" CACHE STRING "This is my variable")

if(SDL_LOCAL)
    set(SDL_DIR /home/bmonkey/Apps/Graphics/SDL)
ENDIF()

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(APP_DIR ${CMAKE_SOURCE_DIR}/examples)

#Optimization / Debug
IF(RELEASE)
    set(CMAKE_CXX_FLAGS "-O3 -msse4 -mssse3") #Optimized
ELSE()
    set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
ENDIF()

#Set c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

#check for qt
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

#check for opengl3 headers
function(download_gl_header GL_HEADER)
		execute_process(
			WORKING_DIRECTORY include/GL3
			COMMAND wget http://www.opengl.org/registry/api/${GL_HEADER}
		)
endfunction(download_gl_header)

IF(EXISTS /usr/include/GL3)
	message("OpenGL 3 Headers in /usr/include")
ELSE()
	message("No OpenGL 3 Headers found in /usr! Using local ones")
	IF(NOT EXISTS include/GL3)
		execute_process(COMMAND mkdir include/GL3)
	ENDIF()
	SET(GL_HEADERS gl3.h glext.h glxext.h)
	foreach(GL_HEADER ${GL_HEADERS})
    	IF(NOT EXISTS include/GL3/${GL_HEADER})
		    download_gl_header(${GL_HEADER})
	    ENDIF()
    endforeach(GL_HEADER)

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
	    ${SDL_DIR}/include
	else()
	   /usr/include/SDL/
	endif()
)

#Add liblub sources
set(SOURCE_DIRS
        Renderer Material Mesh Scene
)
foreach(SOURCE_DIR ${SOURCE_DIRS})
    file(GLOB_RECURSE GLOB_RESULT ${SRC_DIR}/${SOURCE_DIR}/*.cpp)
    set(SOURCES ${SOURCES} ${GLOB_RESULT})
endforeach(SOURCE_DIR)

#choose media layer
IF(WITH_SDL)
    SET(WINDOW_TYPE SDL)
else()
  SET(WINDOW_TYPE XCB)
endif()

ADD_DEFINITIONS(-DWITH_${WINDOW_TYPE})
file(GLOB_RECURSE GLOB_RESULT ${SRC_DIR}/${SOURCE_DIR}/Window/${WINDOW_TYPE}/*.cpp)
set(SOURCES ${SOURCES} ${GLOB_RESULT})

add_library(lub ${SOURCES})

#link
target_link_libraries(lub ${OPENGL_LIB} freeimageplus ${QT_LIBRARIES} assimp)

IF(WITH_SDL)
    if(SDL_LOCAL)
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${SDL_DIR}/build/.libs")
	    target_link_libraries(lub ${SDL_DIR}/build/.libs/libSDL.a)
    else()
		target_link_libraries(lub SDL)
	endif()
ELSE()
	target_link_libraries(lub X11-xcb xcb-keysyms)
ENDIF()

#build examples
file(GLOB_RECURSE EXAMPLES ${APP_DIR}/*.cpp)

foreach(EXAMPLE_PATH ${EXAMPLES})
    STRING(REGEX REPLACE "\\.cpp" "" EXAMPLE_NAME "${EXAMPLE_PATH}")
    STRING(REGEX REPLACE "^.*/([^/]*)\$" "\\1" EXAMPLE_NAME "${EXAMPLE_NAME}")
    add_executable(${EXAMPLE_NAME} "${EXAMPLE_PATH}")
    target_link_libraries(${EXAMPLE_NAME} lub)
endforeach(EXAMPLE_PATH)
