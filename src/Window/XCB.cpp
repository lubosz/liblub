/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */
#include "MediaLayer.h"

#include <iostream>


#include <stdio.h>
#include <stdlib.h>

#include "Camera.h"

#include <xcb/xcb_atom.h>

MediaLayer::MediaLayer(){
	//FPS Stuff
	fps_lasttime = 0; //the last recorded time.
	fps_frames = 0; //frames passed since the last recorded fps.

	fullscreen = false;
	grab = false;
	quit = false;

	mouseLastX = 0;
	mouseLastY = 0;
}

void MediaLayer::init(string title, unsigned width, unsigned height) {

	programTile = title;
	this->width = width;
	this->height = height;

	initScreen();
	initFrameBuffer();
	createGLContext();
	createColorMap();

	createWindow();

#ifdef XI2
	input = new Input(display, window);
#else
	input = new Input(connection);
#endif

}

MediaLayer::~MediaLayer() {
    /* Cleanup */
    glXDestroyWindow(display, glxwindow);
    xcb_destroy_window(connection, window);
    glXDestroyContext(display, context);
    XCloseDisplay(display);
}

void MediaLayer::initScreen(){
    /* Open Xlib Display */
    display = XOpenDisplay(0);
    if(!display) error("Can't open display\n");

    default_screen = DefaultScreen(display);

    /* Get the XCB connection from the display */
    connection = XGetXCBConnection(display);
    if(!connection) error("Can't get xcb connection from display\n");

    /* Acquire event queue ownership */
    XSetEventQueueOwner(display, XCBOwnsEventQueue);

    /* Find XCB screen */
   screen = 0;
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(connection));
    for(int screen_num = default_screen;
        screen_iter.rem && screen_num > 0;
        --screen_num, xcb_screen_next(&screen_iter));
    screen = screen_iter.data;

}

void MediaLayer::initFrameBuffer(){
    /* Query framebuffer configurations */
    GLXFBConfig *fb_configs = 0;
    int num_fb_configs = 0;
    fb_configs = glXGetFBConfigs(display, default_screen, &num_fb_configs);
    if(!fb_configs || num_fb_configs == 0) error("glXGetFBConfigs failed\n");
    fb_config = fb_configs[0];
}

void MediaLayer::createGLContext() {
	/* Initialize window and OpenGL context */
	visualID = 0;

	/* Select first framebuffer config and query visualID */
	glXGetFBConfigAttrib(display, fb_config, GLX_VISUAL_ID, &visualID);

	/* Simple OpenGL context */
	//context = glXCreateNewContext(display, fb_config, GLX_RGBA_TYPE, 0, True);

	int attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB,
			LIBLUB_GL_MAJOR_VERION,
			GLX_CONTEXT_MINOR_VERSION_ARB,
			LIBLUB_GL_MINOR_VERION,
			0
	};

	//TODO: WHY!?!
	//Get a pointer to the context creation function for GL 3.0
	PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((GLubyte *) "glXCreateContextAttribsARB");

	if (!glXCreateContextAttribs)
		error("GL 3.x is not supported");

	// Create a GL 3.x context
	context = glXCreateContextAttribs(display, fb_config, NULL, True, attribs);

	if(!context) error("glXCreateNewContext failed\n");
}

void MediaLayer::createColorMap() {
	/* Create XID's for colormap and window */
	colormap = xcb_generate_id(connection);
	window = xcb_generate_id(connection);

	/* Create colormap */
	xcb_create_colormap(
			connection,
			XCB_COLORMAP_ALLOC_NONE,
			colormap,
			screen->root,
			visualID
	);
}

void MediaLayer::createWindow() {
	eventmask =
			//XCB_EVENT_MASK_EXPOSURE |
			XCB_EVENT_MASK_KEY_PRESS |
			XCB_EVENT_MASK_POINTER_MOTION |
			XCB_EVENT_MASK_KEY_RELEASE
			/*
			|
	 		XCB_EVENT_MASK_BUTTON_PRESS |
			XCB_EVENT_MASK_BUTTON_RELEASE |
			XCB_EVENT_MASK_ENTER_WINDOW |
			XCB_EVENT_MASK_LEAVE_WINDOW
			 */
			;
	uint32_t valuelist[] = { eventmask, colormap, 0 };
	uint32_t valuemask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

	xcb_create_window(
			connection,
			XCB_COPY_FROM_PARENT,
			window,
			screen->root,
			0, 0,
			width, height,
			0,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			visualID,
			valuemask,
			valuelist
	);

	setWindowTitle(programTile);

	// NOTE: window must be mapped before glXMakeContextCurrent
	xcb_map_window(connection, window);

	/* Create GLX Window */
	drawable = 0;

	glxwindow = glXCreateWindow(display, fb_config, window, 0);

	if (!window)
		error("glXDestroyContext failed\n");

	drawable = glxwindow;

	/* make OpenGL context current */
	if (!glXMakeContextCurrent(display, drawable, drawable, context))
		error("glXMakeContextCurrent failed\n");

	const static uint32_t values[] = { XCB_STACK_MODE_ABOVE };

	/* Move the window on the top of the stack */
	xcb_configure_window(
			connection,
			window,
			XCB_CONFIG_WINDOW_STACK_MODE,
			values
	);
}

/* A simple function that prints a message, the error code returned by SDL, and quits the application */
void MediaLayer::error(string msg)
{
	cerr << msg << "\n";
	exit(0);
}

void MediaLayer::swapBuffers(){
	glXSwapBuffers(display, drawable);
}

void MediaLayer::toggleFullScreen(){
	if(fullscreen){

		fullscreen = false;
	}else{

		fullscreen = true;
	}
}

void MediaLayer::renderLoop(){
    while (!quit) {
    	input->eventLoop();
    	RenderEngine::Instance().display();

    	swapBuffers();
    	getFPS();
     }
}

void MediaLayer::getFPS(){
    struct timespec now;
    static struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &now);
    unsigned ticks = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;

    //FPS Stuff
    fps_frames++;
    if (fps_lasttime < ticks - 1000){
           fps_lasttime = ticks;
           fps_current = fps_frames;
           fps_frames = 0;
    }
    //windowTitle.clear();
    stringstream windowTitle;
    windowTitle << programTile << " - FPS: " << fps_current;
	setWindowTitle(windowTitle.str());
}

void MediaLayer::setWindowTitle(string title){
    xcb_change_property (connection,
                         XCB_PROP_MODE_REPLACE,
                         window,
                         WM_NAME,
                         STRING,
                         8,
                         title.length(),
                         title.c_str()
    );

/* set the title of the window icon */
/*
    char *iconTitle = "Hello World ! (iconified)";
    xcb_change_property (connection,
                         XCB_PROP_MODE_REPLACE,
                         windows,
                         WM_ICON_NAME,
                         STRING,
                         8,
                         strlen(iconTitle),
                         iconTitle);
*/

}