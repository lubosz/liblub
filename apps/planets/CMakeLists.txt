cmake_minimum_required(VERSION 2.8)
project(liblub)

if(WITH_Qt)

file(GLOB PLANETS_CPP_FILES *.cpp)

#moc qt stuff
set(PLANETS_MOC_INFILES 
    PlanetWidget.h
    PlanetsApp.h
)

qt4_wrap_cpp(PLANETS_MOC_OUTFILES ${PLANETS_MOC_INFILES})
add_executable(liblub-planets ${PLANETS_CPP_FILES} ${PLANETS_MOC_OUTFILES})
target_link_libraries(liblub-planets lub)
install(TARGETS liblub-planets DESTINATION "${CMAKE_INSTALL_BINDIR}")

endif()
