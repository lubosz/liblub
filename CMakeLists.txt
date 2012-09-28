cmake_minimum_required(VERSION 2.8)
project(liblub)

# For custom cmake modules.
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

#Set Options
option(WITH_Qt "Use Qt window/input backend" ON)
option(WITH_XCB "Use XCB (X11) window/input backend" ON)

option(CHECKSTYLE "Check Style" OFF)
option(BUILD_TESTS "Build Tests" OFF)

option(USE_OPENGL3 "Use OpenGL3+ Calls (can be disabled to build with mesa)" On)

option(USE_GLEW "Use Qt Windows For All Apps" OFF)

if(LIBLUB_WINDOWS)
    set(WITH_XCB 0)
    set(USE_GLEW 1)
endif()

if(USE_GLEW)
    message("GLEW only works with Qt window backend")
    ADD_DEFINITIONS(-DUSE_GLEW)
endif()

if(USE_OPENGL3)
    ADD_DEFINITIONS(-DUSE_OPENGL3)
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

function(add_cxxflag FLAG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}" PARENT_SCOPE)
endfunction(add_cxxflag)

#check if faster
add_cxxflag("-ffast-math") 

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    #Debug
    message("On Debug Build")
    add_cxxflag("-O0") 
    add_cxxflag("-g3") 
    add_definitions(-D_DEBUG)
else()
    #Release
    message("On Release Build")
    add_cxxflag("-O3")
    add_definitions(-D_NDEBUG)
endif()

#Set c++1
add_cxxflag("-std=gnu++11")

#Warnings

#Ignore Qt Headers, because they warn a lot
add_cxxflag("-isystem /usr/include/QtCore")
add_cxxflag("-isystem /usr/include/QtGui")
add_cxxflag("-isystem /usr/include/QtXml")
add_cxxflag("-isystem /usr/include/qt4/QtCore")
add_cxxflag("-isystem /usr/include/qt4/QtGui")

add_cxxflag("-Wall")
add_cxxflag("-Wextra")
add_cxxflag("-Wuninitialized")
add_cxxflag("-Wold-style-cast")
add_cxxflag("-Woverloaded-virtual")
add_cxxflag("-Wformat=2")
add_cxxflag("-Wundef")
add_cxxflag("-Wpointer-arith")
add_cxxflag("-Wcast-qual")
add_cxxflag("-Wlogical-op")
add_cxxflag("-Woverflow")
add_cxxflag("-Wmissing-declarations")
add_cxxflag("-Wredundant-decls")

#add_cxxflag("-Wconversion")
#add_cxxflag("-Wsign-conversion")
#add_cxxflag("-Winline")
#add_cxxflag("-Weffc++")
#add_cxxflag("-Wsuggest-attribute=const")
#support ISO C++ standard (remove GNU extensions)
#add_cxxflag("-ansi")
#warnings for strict ISO C and ISO C++
#add_cxxflag("-pedantic")

if(WARNINGS)
	add_cxxflag("-Werror")
endif()

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
if (GCC_VERSION VERSION_GREATER 4.6 OR GCC_VERSION VERSION_EQUAL 4.6)
	message(STATUS "GCC Version >= 4.6")
else()
	message(STATUS "GCC Version < 4.6, try to upgrade")      
endif()

find_package(EGLUT REQUIRED)

find_package(Qt4 REQUIRED)

include_directories(
	src
	${QT_INCLUDES}
)

#check for opengl corearb headers
function(download_gl_header GL_HEADER)
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/include/GL/${GL_HEADER})
        execute_process(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/include/GL
            COMMAND wget http://www.opengl.org/registry/api/${GL_HEADER}
    )
    endif()
endfunction(download_gl_header)

if(EXISTS /usr/include/GL/glcorearb.h AND NOT LIBLUB_WINDOWS)
	message("Core Profile API Header found in /usr/include")
else()
	message("No Core Profile API Header in /usr. Using local ones / downloading.")
	if(NOT EXISTS include/GL)
		execute_process(COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/include/GL)
	endif()
	#set(GL_HEADERS glcorearb.h glext.h glxext.h)
	set(GL_HEADERS glcorearb.h)
	foreach(GL_HEADER ${GL_HEADERS})
		    download_gl_header(${GL_HEADER})
    endforeach(GL_HEADER)

	include_directories(include/)
endif()

include(GNUInstallDirs)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(apps)
ADD_SUBDIRECTORY(demos)
ADD_SUBDIRECTORY(tests)

#install
set(BINPERMISSIONS PERMISSIONS
	OWNER_READ OWNER_WRITE OWNER_EXECUTE
	GROUP_READ GROUP_EXECUTE
	WORLD_READ WORLD_EXECUTE)

install(FILES "${CMAKE_SOURCE_DIR}/bin/liblub-examples" DESTINATION "${CMAKE_INSTALL_BINDIR}" ${BINPERMISSIONS})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/media" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/liblub")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/liblub")

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
