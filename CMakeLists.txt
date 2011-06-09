cmake_minimum_required(VERSION 2.8)
project(liblub)

#Set Options
OPTION(WITH_SFML "Use SFML 2" OFF)
OPTION(WITH_SDL "Use SDL 1.3" OFF)
OPTION(SDL_LOCAL "SDL Local" OFF)
OPTION(RELEASE "Release" OFF)
OPTION(USE_FBO "Use Framebuffer Objects" ON)
OPTION(CHECKSTYLE "Check Style" OFF)
OPTION(BUILD_TESTS "Build Tests" OFF)

if(USE_GLEW)
ADD_DEFINITIONS(-DUSE_GLEW)
endif()

if(USE_FBO)
    ADD_DEFINITIONS(-DUSE_FBO)
endif()

SET(SDL_DIR "" CACHE PATH "SDL Local DIR")

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(APP_DIR ${CMAKE_SOURCE_DIR}/apps)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
#Optimization / Debug
IF(RELEASE)
    set(CMAKE_CXX_FLAGS "-O3 -msse4 -mssse3") #Optimized
ELSE()
    set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
ENDIF()

#Set c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

#Warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Woverloaded-virtual -Wuninitialized")

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
if (GCC_VERSION VERSION_GREATER 4.6 OR GCC_VERSION VERSION_EQUAL 4.6)
	message(STATUS "GCC Version >= 4.6")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-attribute=const")
else()
	message(STATUS "GCC Version < 4.6, try to upgrade")      
endif()


#check for qt
FIND_PACKAGE(Qt4 REQUIRED)
find_package(Grantlee REQUIRED)
set(QT_USE_QTXML true)
set(QT_USE_QTUITOOLS true)
#set(QT_USE_QTOPENGL false)
set(QT_USE_QTOPENGL true)
INCLUDE(${QT_USE_FILE})
find_package(OpenGL)
if(NOT OPENGL_FOUND)
	message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)

#moc qt stuff
set(MOC_INFILES 
    src/Window/Qt/Window.h 
    src/Window/Qt/glWidget.h 
    src/Planets/Planet.h 
    src/Window/Qt/PlanetWidget.h
    src/Window/Qt/FloatEditorWidget.h
)
qt4_wrap_cpp (MOC_OUTFILES ${MOC_INFILES})

#check for opengl3 headers
function(download_gl_header GL_HEADER)
    IF(NOT EXISTS src/Renderer/GL3/${GL_HEADER})
		execute_process(
			WORKING_DIRECTORY src/Renderer/GL3
			COMMAND wget http://www.opengl.org/registry/api/${GL_HEADER}
		)
	ENDIF()
endfunction(download_gl_header)

IF(EXISTS /usr/include/GL3 AND NOT LIBLUB_WINDOWS)
	message("OpenGL 3 Headers in /usr/include")
ELSE()
	message("No OpenGL 3 Headers found in /usr! Using local ones")
	IF(NOT EXISTS src/Renderer/GL3)
		execute_process(COMMAND mkdir src/Renderer/GL3)
	ENDIF()
	SET(GL_HEADERS gl3.h glext.h glxext.h)
	foreach(GL_HEADER ${GL_HEADERS})
		    download_gl_header(${GL_HEADER})
    endforeach(GL_HEADER)

	include_directories(src/Renderer/)
ENDIF()


#Check for nvidia libGL in ubuntu
IF(EXISTS "/usr/lib/nvidia-current/libGL.so")
	message("Taking Ubuntu Nvidia libGL")
	SET(OPENGL_LIB /usr/lib/nvidia-current/libGL.so)
ELSE()
	message("Taking system libGL")
	#SET(OPENGL_LIB GL)
	#set(OPENGL_LIB opengl32)
	set(OPENGL_LIB ${OPENGL_LIBRARY})
ENDIF() 

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
        Renderer Material Mesh Scene System Planets
)
foreach(SOURCE_DIR ${SOURCE_DIRS})
    file(GLOB_RECURSE GLOB_RESULT ${SRC_DIR}/${SOURCE_DIR}/*.cpp)
    set(SOURCES ${SOURCES} ${GLOB_RESULT})
endforeach(SOURCE_DIR)

#choose window lib
IF(WITH_SDL)
    SET(WINDOW_TYPE SDL)
elseif(WITH_SFML)
    SET(WINDOW_TYPE SFML)
else()
  SET(WINDOW_TYPE XCB)
endif()

SET(WINDOW_TYPE Qt)

ADD_DEFINITIONS(-DWITH_${WINDOW_TYPE})
file(GLOB_RECURSE GLOB_RESULT ${SRC_DIR}/${SOURCE_DIR}/Window/${WINDOW_TYPE}/*.cpp)

set(SOURCES ${SOURCES} ${GLOB_RESULT})
set(SOURCES ${SOURCES} ${MOC_OUTFILES})
add_library(lub ${SOURCES})

#ADD_DEFINITIONS(-DUSE_FREEIMAGE)

#link
target_link_libraries(lub ${OPENGL_LIB}

#freeimageplus 
${QT_LIBRARIES} 
${Grantlee_CORE_LIBRARIES}
assimp
#/home/bmonkey/Desktop/Assimp32.dll
#/usr/i486-mingw32/lib/libassimp.dll
#GLEW
)

if(USE_GLEW)
    if(LIBLUB_WINDOWS)
        target_link_libraries(lub glew32)
    else()
        target_link_libraries(lub GLEW)
    endif()
endif()

IF(WITH_SDL)
    if(SDL_LOCAL)
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${SDL_DIR}/build/.libs")
	    target_link_libraries(lub ${SDL_DIR}/build/.libs/libSDL.a)
    else()
		target_link_libraries(lub SDL)
	endif()
ELSEIF(WITH_SFML)
	target_link_libraries(lub sfml-graphics)
ELSEIF(WITH_XCB)
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


#build tests
if(BUILD_TESTS)
    file(GLOB_RECURSE TESTS ${TEST_DIR}/*.cpp)
    foreach(TEST_PATH ${TESTS})
        STRING(REGEX REPLACE "\\.cpp" "" TEST_NAME "${TEST_PATH}")
        STRING(REGEX REPLACE "^.*/([^/]*)\$" "\\1" TEST_NAME "${TEST_NAME}")
        add_executable(${TEST_NAME} "${TEST_PATH}")
        target_link_libraries(${TEST_NAME} lub cppunit)
    endforeach(TEST_PATH)
endif()

#check code stlye
IF(EXISTS "/usr/bin/python2")
    SET(PYTHON_VER "python2")
ELSE()
    SET(PYTHON_VER "python")
ENDIF()

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
