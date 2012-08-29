cmake_minimum_required(VERSION 2.8)
project(liblub)

file(GLOB_RECURSE GRAVITY_CPP_FILES *.cpp)

#moc qt stuff
set(GRAVITY_MOC_INFILES
    gravity.h
)

qt4_wrap_cpp(GRAVITY_MOC_OUTFILES ${GRAVITY_MOC_INFILES})
add_executable(gravity ${GRAVITY_CPP_FILES} ${NODES_CPP_FILES} ${GRAVITY_MOC_OUTFILES})
target_link_libraries(gravity lub)
