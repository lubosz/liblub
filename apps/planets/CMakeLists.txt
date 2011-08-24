cmake_minimum_required(VERSION 2.8)
project(liblub)

file(GLOB_RECURSE CPP_FILES *.cpp)

#moc qt stuff
set(PLANETS_MOC_INFILES 
    PlanetWindow.h 
    glWidget.h 
    Planet.h 
    PlanetWidget.h
    FloatEditorWidget.h
)

qt4_wrap_cpp(PLANETS_MOC_OUTFILES ${PLANETS_MOC_INFILES})
add_executable(planets ${CPP_FILES} ${PLANETS_MOC_OUTFILES})
target_link_libraries(planets lub)
