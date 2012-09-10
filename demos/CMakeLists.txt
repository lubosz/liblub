cmake_minimum_required(VERSION 2.8)
project(liblub)

find_package(Qt4 REQUIRED)

#build examples
file(GLOB_RECURSE DEMOS *.cpp)

#moc qt
set(MOC_INFILES
    gravity/gravity.h
)
qt4_wrap_cpp(MOC_OUTFILES ${MOC_INFILES})

add_library(lubDemos ${DEMOS} ${MOC_OUTFILES})
target_link_libraries(lubDemos lub)
install(TARGETS lubDemos DESTINATION "${CMAKE_INSTALL_LIBDIR}")
