set(EGL_LIBS EGL GL)

include_directories(../../demos)

add_library (eglut_x11 
	eglut.c 
	eglut_x11.c)
target_link_libraries (eglut_x11 ${EGL_LIBS} X11)

add_library (eglut_screen eglut.c eglut_screen.c)
target_link_libraries (eglut_screen ${EGL_LIBS})

add_library (eglut_wayland eglut.c eglut_wayland.c)
target_link_libraries (eglut_wayland 
	wayland-client 
	wayland-egl 
	${EGL_LIBS})

add_executable(backend-egl-drm backend-egl.c)
target_link_libraries(backend-egl-drm eglut_screen)

add_executable(backend-egl-x11 backend-egl.c)
target_link_libraries(backend-egl-x11 eglut_x11)

add_executable(backend-egl-wayland backend-egl.c)
target_link_libraries(backend-egl-wayland eglut_wayland)
