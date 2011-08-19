cmake_minimum_required(VERSION 2.8)
project(liblub)

file(GLOB_RECURSE CPP_FILES *.cpp)

#moc qt stuff
set(MOC_INFILES 
    src/Window/Qt/Window.h 
    src/Window/Qt/glWidget.h 
    src/Planets/Planet.h 
    src/Window/Qt/PlanetWidget.h
    src/Window/Qt/FloatEditorWidget.h
)

qt4_wrap_cpp (MOC_OUTFILES ${MOC_INFILES})

add_executable(planets ${OC_OUTFILES} ${CPP_FILES})
target_link_libraries(planets lub)
