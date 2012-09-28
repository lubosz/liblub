find_package(Qt4 REQUIRED)

#build examples
file(GLOB EXAMPLES *.cpp)

set(EGL_BACKENDS
	x11 wayland drm
)

foreach(BACKEND ${EGL_BACKENDS})

	string(TOUPPER ${BACKEND} UP)

	if(EGLUT_${UP}_FOUND)
    add_executable(liblub-view-${BACKEND} egl.cpp)
		target_link_libraries(liblub-view-${BACKEND} eglut_screen lubDemos)
		install(TARGETS liblub-view-${BACKEND} DESTINATION "${CMAKE_INSTALL_BINDIR}")
	else ()
		message("Will not build ${BACKEND} backend")
	endif()
endforeach(BACKEND)

add_executable(liblub-view-xcb xcb.cpp)
target_link_libraries(liblub-view-xcb lubDemos lubApp)
install(TARGETS liblub-view-xcb DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(liblub-view-qt qt.cpp)
target_link_libraries(liblub-view-qt lubDemos lubApp)
install(TARGETS liblub-view-qt DESTINATION "${CMAKE_INSTALL_BINDIR}")
