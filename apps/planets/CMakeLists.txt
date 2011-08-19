cmake_minimum_required(VERSION 2.8)
project(liblub)

file(GLOB_RECURSE CPP_FILES *.cpp)

#moc qt stuff
set(MOC_INFILES 
    Window.h 
    glWidget.h 
    Planet.h 
    PlanetWidget.h
    FloatEditorWidget.h
)

qt4_wrap_cpp(MOC_OUTFILES ${MOC_INFILES})
add_executable(planets ${CPP_FILES} ${MOC_OUTFILES})
target_link_libraries(planets lub)
