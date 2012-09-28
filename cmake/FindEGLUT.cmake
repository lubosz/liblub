# Try to find eglut library and include path.
# Once done this will define
#
# EGLUT_FOUND
# EGLUT_INCLUDE_DIR
# EGLUT_LIBRARIES
#


find_path(EGLUT_INCLUDE_DIR EGL/eglut.h
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		DOC "The directory where EGL/eglut.h resides")

mark_as_advanced(EGLUT_INCLUDE_DIR)

		
set(EGL_BACKENDS
	x11 wayland screen
)

foreach(BACKEND ${EGL_BACKENDS})

find_library(EGLUT_${BACKEND}_LIBRARY
		NAMES eglut_${BACKEND}
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		DOC "The eglut ${BACKEND} library")
		
mark_as_advanced(EGLUT_${BACKEND}_LIBRARY)
endforeach(BACKEND)

# handle the QUIETLY and REQUIRED arguments and set EGLUT_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(EGLUT_X11 DEFAULT_MSG EGLUT_x11_LIBRARY EGLUT_INCLUDE_DIR)
find_package_handle_standard_args(EGLUT_WAYLAND DEFAULT_MSG EGLUT_wayland_LIBRARY EGLUT_INCLUDE_DIR)
find_package_handle_standard_args(EGLUT_DRM DEFAULT_MSG EGLUT_screen_LIBRARY EGLUT_INCLUDE_DIR)

if(EGLUT_X11_FOUND)
	set(EGLUT_X11_LIBRARIES ${EGLUT_x11_LIBRARY})
endif()

if(EGLUT_DRM_FOUND)
	set(EGLUT_X11_LIBRARIES ${EGLUT_x11_LIBRARY})
endif()

if(EGLUT_WAYLAND_FOUND)
	set(EGLUT_X11_LIBRARIES ${EGLUT_x11_LIBRARY})
endif()




		

