#Editor Lib
file(GLOB_RECURSE EDITOR_SRC *.cpp)

set(EDITOR_MOC_INFILES
    TextureModel.h
    PassModel.h
    TargetModel.h
    Nodes/GraphWidget.h
    PlanetWidget.h
    FloatEditorWidget.h
)
qt4_wrap_cpp(EDITOR_MOC_OUTFILES ${EDITOR_MOC_INFILES})
add_library(lubEdit ${EDITOR_SRC} ${EDITOR_MOC_OUTFILES})

target_link_libraries(lubEdit lubApp)
install(TARGETS lubEdit DESTINATION "${CMAKE_INSTALL_LIBDIR}")
