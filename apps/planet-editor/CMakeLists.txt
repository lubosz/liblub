cmake_minimum_required(VERSION 2.8)
project(liblub)

file(GLOB_RECURSE EDITOR_CPP_FILES *.cpp)

include_directories(
    ../../demos
)

set(EDITOR_MOC_INFILES
    planet-editor.h
    PlanetWidget.h
)

qt4_wrap_cpp(EDITOR_MOC_OUTFILES ${EDITOR_MOC_INFILES})
add_executable(liblub-planet-editor ${EDITOR_CPP_FILES} ${EDITOR_MOC_OUTFILES})
target_link_libraries(liblub-planet-editor lub lubDemos)
install(TARGETS liblub-planet-editor DESTINATION "${CMAKE_INSTALL_BINDIR}")
