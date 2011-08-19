cmake_minimum_required(VERSION 2.8)
project(liblub)

#Set Options
option(WITH_Qt "Use Qt window/input backend" ON)

if(LIBLUB_WINDOWS)
option(WITH_XCB "Use XCB (X11) window/input backend" OFF)
else()
option(WITH_XCB "Use XCB (X11) window/input backend" ON)
endif()

option(WITH_SFML "Use SFML 2 window/input backend" OFF)
option(WITH_SDL "Use SDL 1.3 window/input backend" OFF)
option(SDL_LOCAL "SDL Local" OFF)
option(RELEASE "Release" OFF)
option(USE_FBO "Use Framebuffer Objects" ON)
option(CHECKSTYLE "Check Style" OFF)
option(BUILD_TESTS "Build Tests" OFF)

if(USE_GLEW)
ADD_DEFINITIONS(-DUSE_GLEW)
endif()

if(USE_FBO)
    ADD_DEFINITIONS(-DUSE_FBO)
endif()

set(SDL_DIR "" CACHE PATH "SDL Local DIR")

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
#Optimization / Debug
if(RELEASE)
    set(CMAKE_CXX_FLAGS "-O3 -msse4 -mssse3") #Optimized
else()
    set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
endif()

#Set c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

#Warnings
if(WARNINGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Woverloaded-virtual -Wuninitialized")
endif()
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
if (GCC_VERSION VERSION_GREATER 4.6 OR GCC_VERSION VERSION_EQUAL 4.6)
	message(STATUS "GCC Version >= 4.6")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-attribute=const")
else()
	message(STATUS "GCC Version < 4.6, try to upgrade")      
endif()


#check for qt
find_package(Qt4 REQUIRED)
find_package(Grantlee REQUIRED)
set(QT_USE_QTXML true)
set(QT_USE_QTUITOOLS true)
#set(QT_USE_QTOPENGL false)
set(QT_USE_QTOPENGL true)
include(${QT_USE_FILE})
find_package(OpenGL)
if(NOT OPENGL_FOUND)
	message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)

#check for opengl3 headers
function(download_gl_header GL_HEADER)
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/src/Renderer/GL3/${GL_HEADER})
        execute_process(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/Renderer/GL3
            COMMAND wget http://www.opengl.org/registry/api/${GL_HEADER}
    )
    endif()
endfunction(download_gl_header)

if(EXISTS /usr/include/GL3 AND NOT LIBLUB_WINDOWS)
	message("OpenGL 3 Headers in /usr/include")
else()
	message("No OpenGL 3 Headers found in /usr! Using local ones")
	if(NOT EXISTS src/Renderer/GL3)
		execute_process(COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/src/Renderer/GL3)
	endif()
	set(GL_HEADERS gl3.h glext.h glxext.h)
	foreach(GL_HEADER ${GL_HEADERS})
		    download_gl_header(${GL_HEADER})
    endforeach(GL_HEADER)

	include_directories(src/Renderer/)
endif()


#Check for nvidia libGL in ubuntu
if(EXISTS "/usr/lib/nvidia-current/libGL.so")
	message("Taking Ubuntu Nvidia libGL")
	SET(OPENGL_LIB /usr/lib/nvidia-current/libGL.so)
else()
	message("Taking system libGL")
	#SET(OPENGL_LIB GL)
	#set(OPENGL_LIB opengl32)
	set(OPENGL_LIB ${OPENGL_LIBRARY})
endif()

include_directories(
	src
	${QT_INCLUDES}	
	if(SDL_LOCAL)
	    ${SDL_DIR}/include
	else()
	   if(LIBLUB_WINDOWS)
	      /usr/i486-mingw32/include/SDL
	   else()
	      /usr/include/SDL/
	   endif()
	endif()
)

#Add liblub sources
set(SOURCE_DIRS
        Renderer Material Mesh Scene System
)
foreach(SOURCE_DIR ${SOURCE_DIRS})
    file(GLOB_RECURSE GLOB_RESULT ${SRC_DIR}/${SOURCE_DIR}/*.cpp)
    set(SOURCES ${SOURCES} ${GLOB_RESULT})
endforeach(SOURCE_DIR)

# choose window lib
if(WITH_SDL)
	if(SDL_LOCAL)
		list(APPEND LIBS ${SDL_DIR}/build/.libs/libSDL.a)
	else()
		list(APPEND LIBS SDL)
	endif()
    set(WINDOW_TYPES ${WINDOW_TYPES} SDL)
endif()

if(WITH_SFML)
	list(APPEND LIBS sfml-graphics)
    set(WINDOW_TYPES ${WINDOW_TYPES} SFML)
endif()

if(WITH_Qt)
    set(WINDOW_TYPES ${WINDOW_TYPES} Qt)
endif()

if(WITH_XCB)
	list(APPEND LIBS X11-xcb xcb-keysyms)
    set(WINDOW_TYPES ${WINDOW_TYPES} XCB)
endif()

foreach(WINDOW_TYPE ${WINDOW_TYPES})
    add_definitions(-DWITH_${WINDOW_TYPE})
    file(GLOB_RECURSE GLOB_RESULT ${SRC_DIR}/Window/${WINDOW_TYPE}/*.cpp)
    set(WINDOW_CODE ${WINDOW_CODE} ${GLOB_RESULT})
endforeach()

if(WINDOW_CODE)
else()
    message(FATAL_ERROR "You need to choose a window lib.")
endif()

file(GLOB_RECURSE GLOB_RESULT ${SRC_DIR}/Window/*.h)
set(WINDOW_CODE ${WINDOW_CODE} ${GLOB_RESULT})

set(SOURCES ${SOURCES} ${WINDOW_CODE})
add_library(lub ${SOURCES})

#libs to link
list(APPEND LIBS
    ${OPENGL_LIB}
    ${QT_LIBRARIES}
    ${Grantlee_CORE_LIBRARIES}
    assimp
)

if(LIBLUB_WINDOWS)
    list(APPEND LIBS glew32)
endif()

#link
target_link_libraries(lub ${LIBS})

ADD_SUBDIRECTORY(apps)
ADD_SUBDIRECTORY(tests)

#check code stlye
if(EXISTS "/usr/bin/python2")
    set(PYTHON_VER "python2")
else()
    set(PYTHON_VER "python")
endif()

if(CHECKSTYLE)
    file(GLOB GLOB_RESULT include/*.h)
    set(INCLUDES ${INCLUDES} ${GLOB_RESULT})
    file(GLOB GLOB_RESULT include/common/*.h)
    set(INCLUDES ${INCLUDES} ${GLOB_RESULT})
    
    set(IGNORE "-build/header_guard,-readability/streams")

    add_custom_target(CHECK_STYLE ALL
        COMMAND ${PYTHON_VER} scripts/cpplint.py --filter=${IGNORE} ${SOURCES} ${INCLUDES} ${EXAMPLES}
    )
endif()

add_custom_target(remake
	#clean and compile with 1 thread per core
	COMMAND make clean && rm CMakeCache.txt && cmake ${CMAKE_SOURCE_DIR} -G\"Unix Makefiles\" && make -j`getconf _NPROCESSORS_ONLN`
)
