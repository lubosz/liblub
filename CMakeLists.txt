cmake_minimum_required(VERSION 2.8)
project(liblub)

#Set Options
option(WITH_Qt "Use Qt window/input backend" ON)
option(WITH_XCB "Use XCB (X11) window/input backend" ON)

option(CHECKSTYLE "Check Style" OFF)
option(BUILD_TESTS "Build Tests" OFF)

option(USE_OPENGL3 "Use OpenGL3+ Calls (can be disabled to build with mesa)" On)

option(USE_QT_WINDOWS "Use Qt Windows For All Apps" OFF)
option(USE_GLEW "Use Qt Windows For All Apps" OFF)

if(LIBLUB_WINDOWS)
    set(WITH_XCB 0)
    set(USE_GLEW 1)
    set(USE_QT_WINDOWS 1)
endif()

if(USE_GLEW)
    message("GLEW only works with Qt window backend")
    set(USE_QT_WINDOWS 1)
    ADD_DEFINITIONS(-DUSE_GLEW)
endif()

if(USE_OPENGL3)
    ADD_DEFINITIONS(-DUSE_OPENGL3)
endif()

if(USE_QT_WINDOWS)
    ADD_DEFINITIONS(-DUSE_QT_WINDOWS)
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

set(CMAKE_CXX_FLAGS "-ffast-math") #Faster

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    #Debug
    message("On Debug Build")
    set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
    add_definitions(-D_DEBUG)
else()
    #Release
    message("On Release Build")
    set(CMAKE_CXX_FLAGS "-O3 -msse4 -mssse3") #Optimized
    add_definitions(-D_NDEBUG)
endif()

#Set c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

#Warnings
if(WARNINGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Woverloaded-virtual -Wuninitialized")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast -isystem /usr/include/QtCore")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++ -Wold-style-cast -Winline ")
# support ISO C++ standard (remove GNU extensions)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi")
# warnings for strict ISO C and ISO C++
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
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

if(WITH_Qt)
    set(QT_USE_QTUITOOLS true)
    set(QT_USE_QTOPENGL true)
endif()

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
)

#Add liblub sources
set(SOURCE_DIRS
        Renderer Material Mesh Scene System
)
foreach(SOURCE_DIR ${SOURCE_DIRS})
    #cpps
    file(GLOB_RECURSE GLOB_RESULT ${SRC_DIR}/${SOURCE_DIR}/*.cpp)
    set(SOURCES ${SOURCES} ${GLOB_RESULT})
    #headers
    file(GLOB_RECURSE GLOB_RESULT ${SRC_DIR}/${SOURCE_DIR}/*.h)
    set(SOURCES ${SOURCES} ${GLOB_RESULT})
endforeach(SOURCE_DIR)

#moc qt stuff
set(LIBLUB_MOC_INFILES 
    src/Window/Input.h
    src/Scene/Node.h
)
qt4_wrap_cpp(LIBLUB_MOC_OUTFILES ${LIBLUB_MOC_INFILES})
set(SOURCES ${SOURCES} ${LIBLUB_MOC_OUTFILES})

if(WITH_Qt)
    set(WINDOW_TYPES ${WINDOW_TYPES} Qt)
    set(QT_MOC_INFILES 
        src/Window/Qt/QtWindow.h
        src/Window/Qt/glWidget.h 
        src/Window/Qt/FloatEditorWidget.h
        src/Window/Qt/QtApplication.h
        src/Window/Qt/QtInput.h
    )
    qt4_wrap_cpp(QT_MOC_OUTFILES ${QT_MOC_INFILES})
    set(SOURCES ${SOURCES} ${QT_MOC_OUTFILES})
endif()
if(WITH_XCB)
	list(APPEND LIBS X11-xcb xcb-keysyms xcb)
    set(WINDOW_TYPES ${WINDOW_TYPES} XCB)
    set(XCB_MOC_INFILES 
        src/Window/XCB/XCBInput.h
        src/Window/XCB/XCBApplication.h
    )
    qt4_wrap_cpp(XCB_MOC_OUTFILES ${XCB_MOC_INFILES})
    set(SOURCES ${SOURCES} ${XCB_MOC_OUTFILES})
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
    rt
)


if(USE_GLEW)
    if(LIBLUB_WINDOWS)
        list(APPEND LIBS glew32)
    else()
        list(APPEND LIBS GLEW)
    endif()
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

set(IGNORE "-build/header_guard,-readability/streams")

add_custom_target(style
    COMMAND ${PYTHON_VER} scripts/cpplint.py --filter=${IGNORE} ${SOURCES}
)

add_custom_target(remake
	#clean and compile with 1 thread per core
	COMMAND make clean && rm CMakeCache.txt && cmake ${CMAKE_SOURCE_DIR} -G\"Unix Makefiles\" && make -j`getconf _NPROCESSORS_ONLN`
)
