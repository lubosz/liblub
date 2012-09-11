#Application Lib
#file(GLOB APP_SRC ${SRC_DIR}/*.cpp)

set(APP_MOC_INFILES 
    Input.h
)
qt4_wrap_cpp(APP_MOC_OUTFILES ${APP_MOC_INFILES})
list(APPEND APP_SRC ${APP_MOC_OUTFILES})

if(WITH_Qt)
    set(WINDOW_TYPES ${WINDOW_TYPES} Qt)
    set(QT_MOC_INFILES 
        Qt/QtWindow.h
        Qt/glWidget.h 
        Qt/QtApplication.h
        Qt/QtInput.h
    )
    qt4_wrap_cpp(QT_MOC_OUTFILES ${QT_MOC_INFILES})
    list(APPEND APP_SRC ${QT_MOC_OUTFILES})
endif()
if(WITH_XCB)
	list(APPEND APP_LIBS X11-xcb xcb-keysyms xcb)
    set(WINDOW_TYPES ${WINDOW_TYPES} XCB)
    set(XCB_MOC_INFILES 
        XCB/XCBInput.h
        XCB/XCBApplication.h
    )
    qt4_wrap_cpp(XCB_MOC_OUTFILES ${XCB_MOC_INFILES})
    list(APPEND APP_SRC ${XCB_MOC_OUTFILES})
endif()

foreach(WINDOW_TYPE ${WINDOW_TYPES})
    add_definitions(-DWITH_${WINDOW_TYPE})
    file(GLOB_RECURSE GLOB_RESULT ${WINDOW_TYPE}/*.cpp)
    list(APPEND APP_SRC ${GLOB_RESULT})
endforeach()

if(WINDOW_TYPES)
else()
    message(FATAL_ERROR "You need to choose a window lib.")
endif()

add_library(lubApp ${APP_SRC})

target_link_libraries(lubApp lub ${APP_LIBS})
install(TARGETS lubApp DESTINATION "${CMAKE_INSTALL_LIBDIR}")
