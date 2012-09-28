cmake_minimum_required(VERSION 2.8)
project(liblub)

include_directories(
    ../demos
)

ADD_SUBDIRECTORY(editor)
ADD_SUBDIRECTORY(planet-editor)
ADD_SUBDIRECTORY(viewer)
