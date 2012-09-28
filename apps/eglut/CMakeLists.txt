add_executable(liblub-view-drm backend-egl.cpp)
target_link_libraries(liblub-view-drm eglut_screen lubDemos)

add_executable(liblub-view-x11 backend-egl.cpp)
target_link_libraries(liblub-view-x11 eglut_x11 lubDemos)

add_executable(liblub-view-wayland backend-egl.cpp)
target_link_libraries(liblub-view-wayland eglut_wayland lubDemos)
