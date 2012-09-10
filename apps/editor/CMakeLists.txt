cmake_minimum_required(VERSION 2.8)
project(liblub)

file(GLOB_RECURSE EDITOR_CPP_FILES *.cpp)

#moc qt stuff
set(EDITOR_MOC_INFILES
    editor.h
    TextureModel.h
    PassModel.h
    TargetModel.h
    Nodes/GraphWidget.h
)

qt4_wrap_cpp(EDITOR_MOC_OUTFILES ${EDITOR_MOC_INFILES})
add_executable(editor ${EDITOR_CPP_FILES} ${NODES_CPP_FILES} ${EDITOR_MOC_OUTFILES})
target_link_libraries(editor lub)
install(TARGETS editor DESTINATION "${CMAKE_INSTALL_BINDIR}")
