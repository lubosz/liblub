cmake_minimum_required(VERSION 2.8)
project(liblub)

#check for qt
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

#Check for nvidia-current and fglrx in ubuntu
if(EXISTS "/usr/lib/nvidia-current/libGL.so")
	message("Taking Ubuntu Nvidia libGL")
	SET(OPENGL_LIB /usr/lib/nvidia-current/libGL.so)
elseif(EXISTS "/usr/lib/fglrx/libGL.so")
        message("Taking fglrx ATI libGL")
        SET(OPENGL_LIB /usr/lib/fglrx/libGL.so)
else()
	message("Taking system libGL")
	set(OPENGL_LIB ${OPENGL_LIBRARY})
endif()

#Add liblub sources
set(SOURCE_DIRS
        Common Load Mesh Procedural Renderer Scene Shader System Texture 
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
    Scene/Node.h
    Procedural/Planets/Planet.h
)
qt4_wrap_cpp(LIBLUB_MOC_OUTFILES ${LIBLUB_MOC_INFILES})
add_library(lub ${SOURCES} ${LIBLUB_MOC_OUTFILES})

message(${QT_LIBRARIES})

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
install(TARGETS lub DESTINATION "${CMAKE_INSTALL_LIBDIR}")

add_subdirectory(Application)
add_subdirectory(Editor)

