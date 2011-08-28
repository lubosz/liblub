cmake_minimum_required(VERSION 2.8)
project(liblub)

find_package(Qt4 REQUIRED)

#build examples
file(GLOB EXAMPLES *.cpp)

set(MOC_APPS 
    foo
)

set(SOURCES ${SOURCES} ${LIBLUB_MOC_OUTFILES})

foreach(EXAMPLE_PATH ${EXAMPLES})
    STRING(REGEX REPLACE "\\.cpp" "" EXAMPLE_NAME "${EXAMPLE_PATH}")
    STRING(REGEX REPLACE "^.*/([^/]*)\$" "\\1" EXAMPLE_NAME "${EXAMPLE_NAME}")
    
    foreach(MOC_APP ${MOC_APPS})
        STRING(COMPARE EQUAL ${EXAMPLE_NAME} ${MOC_APP} nceqvar)
        if(${nceqvar})
            qt4_wrap_cpp(EXAMPLE_MOC_FILES "${EXAMPLE_NAME}.h")
            list(APPEND EXAMPLE_PATH ${EXAMPLE_MOC_FILES})
        endif()
    endforeach()
    
    add_executable("${EXAMPLE_NAME}" ${EXAMPLE_PATH})
    target_link_libraries(${EXAMPLE_NAME} lub)
endforeach(EXAMPLE_PATH)

ADD_SUBDIRECTORY(planets)
