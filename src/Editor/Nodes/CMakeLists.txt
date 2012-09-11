cmake_minimum_required(VERSION 2.8)
project(liblub)

if(${USE_QT_WINDOWS})

file(GLOB EDITOR_CPP_FILES *.cpp)

#moc qt stuff
set(EDITOR_MOC_INFILES
    editor.h
    TextureModel.h
    PassModel.h
    TargetModel.h
)

qt4_wrap_cpp(EDITOR_MOC_OUTFILES ${EDITOR_MOC_INFILES})
add_executable(editor ${EDITOR_CPP_FILES} ${EDITOR_MOC_OUTFILES})
target_link_libraries(editor lub)

endif()
