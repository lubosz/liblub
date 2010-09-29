/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */
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

void MediaLayer::init(string title, unsigned width, unsigned height) {
	programTile = title;
	this->width = width;
	this->height = height;

	initScreen();
	initFrameBuffer();
	createGLContext();
	createColorMap();

	createWindow();
	createBlankCursor();

	input = new Input(connection);

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

    printf("white_pixel: %d\n \
			black_pixel: %d\n \
			current_input_masks: %d\n \
			width_in_pixels: %d\n \
			height_in_pixels: %d\n \
			width_in_millimeters: %d\n \
			height_in_millimeters: %d\n \
			min_installed_maps: %d\n \
			max_installed_maps: %d\n \
			backing_stores: %d\n \
			save_unders: %d\n \
			root_depth: %d\n \
			allowed_depths_len: %d\n",

			screen->white_pixel,
			screen->black_pixel,
			screen->current_input_masks,
			screen->width_in_pixels,
			screen->height_in_pixels,
			screen->width_in_millimeters,
			screen->height_in_millimeters,
			screen->min_installed_maps,
			screen->max_installed_maps,
			screen->backing_stores,
			screen->save_unders,
			screen->root_depth,
			screen->allowed_depths_len

    );
    //xcb_window_t   root;
    //xcb_colormap_t default_colormap;
    //xcb_visualid_t root_visual;



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

	context = glXCreateContextAttribsARB(display, fb_config, NULL, True, attribs);

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

/*
    static const uint32_t wm_hints[8] = {
        3, // flags: Input, Initial state
        1, // input: True
        1, // initial state: Normal
        0, 0, 0, 0, 0, // Icon
    };

    xcb_change_property (
    		connection,
    		XCB_PROP_MODE_REPLACE,
    		window,
    		XCB_ATOM_WM_HINTS,
    		XCB_ATOM_WM_HINTS,
            32,
            8,
            wm_hints
    );

	string wm_state_name = "_NET_WM_STATE";
	//string wm_state_fs_name = "_NET_WM_STATE_FULLSCREEN";
	string wm_state_fs_name = "_NET_WM_STATE_MAXIMIZED_VERT";

    //atoms[count++] = data->_NET_WM_STATE_MAXIMIZED_VERT;
    //atoms[count++] = data->_NET_WM_STATE_MAXIMIZED_HORZ;

	xcb_intern_atom_cookie_t wm_state_ck = xcb_intern_atom(connection, 0, wm_state_name.length(), wm_state_name.c_str());
	xcb_intern_atom_cookie_t wm_state_fs_ck = xcb_intern_atom(connection, 0, wm_state_fs_name.length(), wm_state_fs_name.c_str());

	xcb_generic_error_t * error1;
	xcb_generic_error_t * error2;
	xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(connection, wm_state_ck, &error1);
	if (error1)
		fprintf(stderr, "Can't set the screen. Error Code: %i\n",error1->error_code);
	wm_state = reply->atom;
	free(reply);
	//cout << error << "\n";

	reply = xcb_intern_atom_reply(connection, wm_state_fs_ck, &error2);
	if (error2)
		fprintf(stderr, "Can't set the screen. Error Code: %i\n",error2->error_code);
	wm_state_fullscreen = reply->atom;
	free(reply);
*/

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
	glXSwapIntervalSGI(VSync);

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

void MediaLayer::toggleFullScreen() {
	if (fullscreen) {
		cout << "Fullscreen off\n";
		fullscreen = false;
	} else {
		cout << "Fullscreen on\n";
		fullscreen = true;
	}


	/*
	// From EWMH "_WM_STATE"
	xcb_client_message_event_t ev;

	ev.response_type = XCB_CLIENT_MESSAGE;
	ev.format = 32;
	ev.window = window;
	ev.type = wm_state;

	//ev.data.data32[0] = fullscreen ? 1 : 0;
	ev.data.data32[0] = 3; //toggle
	ev.data.data32[1] = wm_state_fullscreen;
	ev.data.data32[2] = 0;
	ev.data.data32[3] = 1;
	ev.data.data32[4] = 0;
*/
/*
	ev.response_type = XCB_CLIENT_MESSAGE;
	ev.type = wm_state;
	ev.format = 32;
	ev.window = window;
    ev.data.data32[0] = 3;
    ev.data.data32[1] = wm_state_fullscreen;
    ev.data.data32[2] = wm_state_fullscreen;
    ev.data.data32[3] = 0l;


	// From ICCCM "Changing Window State"
	xcb_send_event(
			connection,
			0,
			window,
			XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
			(const char *) &ev
	);


	uint32_t borderWidth = 150;
	xcb_configure_window(connection, window, XCB_CONFIG_WINDOW_BORDER_WIDTH, &borderWidth);
	uint32_t newHeight = 150;
	xcb_configure_window(connection, window, XCB_CONFIG_WINDOW_HEIGHT, &newHeight);
	cout << "New height "<< newHeight << "\n";
*/
	/*
	xcb_configure_window(
			connection,
			window,
			XCB_CONFIG_WINDOW_BORDER_WIDTH|XCB_CONFIG_WINDOW_STACK_MODE,
			(uint32_t[]){0,XCB_STACK_MODE_BELOW}
	);
*/



/*
	case XCB_CLIENT_MESSAGE:
	{
		xcb_client_message_event_t* clientEvent =
				(xcb_client_message_event_t*) event;
		if (clientEvent->type == displayData->WM_PROTOCOLS_ATOM) {
			if (clientEvent->data.data32[0]
					== displayData->WM_DELETE_WINDOW_ATOM) {
				handleCloseWindowRequestEvent(env, getTopLevelWindowHandle(env,
						javaDisplay, clientEvent->window));
			} else if (clientEvent->data.data32[0]
					== displayData->NET_WM_PING_ATOM) {
				printf("Got ping\n");
				clientEvent->response_type = XCB_CLIENT_MESSAGE;
				clientEvent->window = screen->root;
				xcb_send_event(connection, false, screen->root,
						XCB_EVENT_MASK_STRUCTURE_NOTIFY
								| XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
						(const char*) &clientEvent);
			}
		}



	xcb_client_message_event_t pingResponse;
	pingResponse.response_type = XCB_CLIENT_MESSAGE;
	pingResponse.format = 32;
	pingResponse.sequence = clientEvent->sequence;
	pingResponse.window = screen->root;
	pingResponse.type = displayData->WM_PROTOCOLS_ATOM;

	for (int i = 0; i < 5; i++) {
		pingResponse.data.data32[i] = clientEvent->data.data32[i];
	}

	xcb_send_event(
			connection,
			false,
			screen->root,
			XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
			(const char*) &pingResponse
	);
*/



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
                         XCB_ATOM_WM_NAME,
                         XCB_ATOM_STRING,
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
		/*
		XGrabPointer(display, window, True, ButtonPressMask,
			GrabModeAsync, GrabModeAsync, window, None, CurrentTime);
			*/
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
		//XUngrabPointer(display, CurrentTime);
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

    //cout << width << " " << height << " " << halfWidth << " " << halfHeight << " " << xRel << " " << yRel << "\n";
}
