cmake_minimum_required(VERSION 2.8)
project(liblub)

find_package(Qt4 REQUIRED)

#build examples
file(GLOB EXAMPLES *.cpp)

include_directories(
    ../demos
)

set(MOC_APPS 
    deferred
    shadow
)

set(SOURCES ${SOURCES} ${LIBLUB_MOC_OUTFILES})

foreach(EXAMPLE_PATH ${EXAMPLES})
    STRING(REGEX REPLACE "\\.cpp" "" EXAMPLE_NAME "${EXAMPLE_PATH}")
    STRING(REGEX REPLACE "^.*/([^/]*)\$" "\\1" EXAMPLE_NAME "${EXAMPLE_NAME}")
    
    set(EXAMPLE_SOURCES ${EXAMPLE_PATH})
    foreach(MOC_APP ${MOC_APPS})
        STRING(COMPARE EQUAL ${EXAMPLE_NAME} ${MOC_APP} nceqvar)
        if(${nceqvar})
            set(EXAMPLE_MOC_FILES "")
            qt4_wrap_cpp(EXAMPLE_MOC_FILES "${EXAMPLE_NAME}.h")
            list(APPEND EXAMPLE_SOURCES ${EXAMPLE_MOC_FILES})
        endif()
    endforeach()
    
    add_executable("${EXAMPLE_NAME}" ${EXAMPLE_SOURCES})
    target_link_libraries(${EXAMPLE_NAME} lub lubDemos)
endforeach(EXAMPLE_PATH)

ADD_SUBDIRECTORY(planets)
ADD_SUBDIRECTORY(editor)
