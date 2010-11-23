/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */
#include <sstream>
#include "MediaLayer.h"
#include "Camera.h"

MediaLayer::MediaLayer(){
	//FPS Stuff
	fps_lasttime = 0; //the last recorded time.
	fps_frames = 0; //frames passed since the last recorded fps.

	fullscreen = false;
	grab = false;
	quit = false;
}

void MediaLayer::init(string title) {
	programTile = title;

	initScreen();
	initFrameBuffer();
	createGLContext();
	createColorMap();

	createWindow();
	createBlankCursor();

	input = new Input(connection);

	//Togle mouse at init
	unsigned halfWidth = width/2;
	unsigned halfHeight = height/2;
	toggleMouseGrab();
	if (grab) XWarpPointer(display, None, window, 0, 0, width, height, halfWidth, halfHeight);

	Camera::Instance().setAspect(float(MediaLayer::Instance().width)/float(MediaLayer::Instance().height));

}

MediaLayer::~MediaLayer() {
    /* Cleanup */
    glXDestroyWindow(display, glxwindow);
    xcb_destroy_window(connection, window);
    glXDestroyContext(display, context);
    XCloseDisplay(display);
}

void MediaLayer::createBlankCursor() {

	cursor = xcb_generate_id(connection);
	xcb_pixmap_t pix = xcb_generate_id(connection);

	xcb_create_pixmap(connection, 1, pix, screen->root, 1, 1);
	xcb_create_cursor(connection, cursor, pix, pix, 0, 0, 0, 0, 0, 0, 1, 1);

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

	width = screen->width_in_pixels;
	height = screen->height_in_pixels;

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

	int attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB,
			LIBLUB_GL_MAJOR_VERION,
			GLX_CONTEXT_MINOR_VERSION_ARB,
			LIBLUB_GL_MINOR_VERION,
			0
	};

	 /* Get a pointer to the context creation function for GL 3.0 */
	PFNGLXCREATECONTEXTATTRIBSARBPROC
			glXCreateContextAttribs =
					(PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress(
							(GLubyte *) "glXCreateContextAttribsARB");
	if (!glXCreateContextAttribs) {
		printf("GL 3.x is not supported");
	}

	context = glXCreateContextAttribs(display, fb_config, NULL, True,
			attribs);

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
			XCB_EVENT_MASK_KEY_PRESS |
			XCB_EVENT_MASK_POINTER_MOTION |
			XCB_EVENT_MASK_KEY_RELEASE
			;
	uint32_t valuelist[] = { eventmask, colormap };
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
	glxwindow = glXCreateWindow(display, fb_config, window, 0);

	if (!glxwindow)
		error("glXCreateWindow failed\n");

	drawable = glxwindow;

	/* make OpenGL context current */
	if (!glXMakeContextCurrent(display, drawable, drawable, context))
		error("glXMakeContextCurrent failed\n");

	//Set swap interval
	PFNGLXSWAPINTERVALSGIPROC
		glXSwapInterval =
					(PFNGLXSWAPINTERVALSGIPROC) glXGetProcAddress(
							(GLubyte *) "glXSwapIntervalSGI");
	if (!glXSwapInterval) {
		printf("VSync is not supported");
	}



	glXSwapInterval(VSync);
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
/*
static void
	wmspec_change_state (bool   add,
			xcb_intern_atom_cookie_t state1,
			xcb_intern_atom_cookie_t state2)
{
  GdkDisplay *display = GDK_WINDOW_DISPLAY (window);
  XClientMessageEvent xclient;

#define _NET_WM_STATE_REMOVE        0    // remove/unset property
#define _NET_WM_STATE_ADD           1    // add/set property
#define _NET_WM_STATE_TOGGLE        2    // toggle property


	xcb_client_message_event_t ev;
	  memset (&ev, 0, sizeof (ev));
	ev.response_type = XCB_CLIENT_MESSAGE;
	ev.window = window;
	ev.type = wm_state;
	ev.format = 32;
	ev.data.data32[0] = 2; //toggle
	ev.data.data32[1] = wm_state_fullscreen;
	ev.data.data32[2] = wm_state_fullscreen;
	ev.data.data32[3] = 0;
	ev.data.data32[4] = 0;

  memset (&xclient, 0, sizeof (xclient));
  xclient.type = ClientMessage;
  xclient.window = GDK_WINDOW_XID (window);
  xclient.message_type = gdk_x11_get_xatom_by_name_for_display (display, "_NET_WM_STATE");
  xclient.format = 32;
  xclient.data.l[0] = add ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
  xclient.data.l[1] = gdk_x11_atom_to_xatom_for_display (display, state1);
  xclient.data.l[2] = gdk_x11_atom_to_xatom_for_display (display, state2);
  xclient.data.l[3] = 0;
  xclient.data.l[4] = 0;

	xcb_send_event(
			connection,
			0,
			window,
			XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
			(const char *) &ev
	);

}
*/
xcb_intern_atom_cookie_t MediaLayer::getCookieForAtom(string state_name){
	return xcb_intern_atom(connection, 0, state_name.length(), state_name.c_str());
}

xcb_atom_t MediaLayer::getReplyAtomFromCookie(xcb_intern_atom_cookie_t cookie){
	xcb_generic_error_t * error;
	xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(connection, cookie, &error);
	if (error)
		fprintf(stderr, "Can't set the screen. Error Code: %i\n",error->error_code);
	return reply->atom;
	//free(reply);
}

//void MediaLayer::changeState(state1, state2)

void MediaLayer::toggleFullScreen() {
	if (fullscreen) {
		cout << "Fullscreen off\n";
	} else {
		cout << "Fullscreen on\n";
	}
	fullscreen = !fullscreen;

	xcb_intern_atom_cookie_t wm_state_ck = getCookieForAtom("_NET_WM_STATE");
	xcb_intern_atom_cookie_t wm_state_fs_ck = getCookieForAtom("_NET_WM_STATE_FULLSCREEN");

#define _NET_WM_STATE_REMOVE        0    // remove/unset property
#define _NET_WM_STATE_ADD           1    // add/set property
#define _NET_WM_STATE_TOGGLE        2    // toggle property

	xcb_client_message_event_t ev;
//	memset (&ev, 0, sizeof (ev));
	ev.response_type = XCB_CLIENT_MESSAGE;
	ev.type = getReplyAtomFromCookie(wm_state_ck);
	ev.format = 32;
	ev.window = window;
//    ev.data.data32[0] = fullscreen ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
	ev.data.data32[0] = _NET_WM_STATE_TOGGLE;
    ev.data.data32[1] = getReplyAtomFromCookie(wm_state_fs_ck);
    ev.data.data32[2] = XCB_ATOM_NONE;
    ev.data.data32[3] = 0;
    ev.data.data32[4] = 0;

	xcb_send_event(
			connection,
			1,
			window,
			XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
			(const char *) &ev
	);

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

    stringstream windowTitle;
    windowTitle << programTile << " - FPS: " << fps_current;
	setWindowTitle(windowTitle.str());
}

void MediaLayer::setWindowTitle(string title){
    xcb_change_property(
    	connection,
    	XCB_PROP_MODE_REPLACE,
    	window,
		XCB_ATOM_WM_NAME,
		XCB_ATOM_STRING,
		8,
		title.length(),
		title.c_str()
	);
}

void MediaLayer::toggleMouseGrab(){
	if (!grab){
		//hide cursor
		uint32_t value_list = cursor;

        xcb_change_window_attributes (
        		connection,
        		window,
                XCB_CW_CURSOR,
                &value_list
        );
		grab = true;
	}else{
	    /* show the default cursor */
		uint32_t value_list = XCB_CURSOR_NONE;
	    xcb_change_window_attributes (
	    		connection,
	    		window,
	    		XCB_CW_CURSOR,
	    		&value_list
	    );
		grab = false;
	}
}

void MediaLayer::mouseLook(int x, int y){
	unsigned halfWidth = width/2;
	unsigned halfHeight = height/2;

	int xRel = x - halfWidth;
	int yRel = y - halfHeight;

	if(!(xRel == 0 && yRel == 0) && grab){
		Camera::Instance().setMouseLook(xRel, yRel);
		if (grab) XWarpPointer(display, None, window, x, y, width, height, halfWidth, halfHeight);
	}
}
