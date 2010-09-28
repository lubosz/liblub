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
#include <X11/extensions/XInput2.h>



MediaLayer::MediaLayer(string title, unsigned width, unsigned height) {

	//FPS Stuff
	fps_lasttime = 0; //the last recorded time.
	fps_frames = 0; //frames passed since the last recorded fps.
	programTile = title;
	fullscreen = false;
	grab = false;
	quit = false;

	mouseLastX = 0;
	mouseLastY = 0;

	this->width = width;
	this->height = height;

	initScreen();
	initFrameBuffer();
	createGLContext();
	createColorMap();




	createWindow();
	setupEventHandlers();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
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



	/* XInput Extension available? */
	int event, myError;
	if (!XQueryExtension(display, "XInputExtension", &opcode, &event, &myError)) error("X Input extension not available.");

	/* Which version of XI2? We support 2.0 */
	int major = 2, minor = 0;
	switch(XIQueryVersion(display, &major, &minor)){
	case Success:
		cout << "Success!";
		printf("Success! Server supports %d.%d\n", major, minor);
		break;
	case BadRequest:
		printf("XI2 not available. Server supports %d.%d\n", major, minor);
		exit(0);
		break;
	default:
		printf("Unknown Error! Server supports %d.%d\n", major, minor);
		cout << "Unknown Error!\n";
		exit(0);
		break;
	}



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

void MediaLayer::setupEventHandlers() {

	/*
	//XI2
	XIEventMask eventmask;
	unsigned char mask[1] = { 0 }; // the actual mask

	eventmask.deviceid = 2;
	eventmask.mask_len = sizeof(mask); // always in bytes
	eventmask.mask = mask;
	// now set the mask
	XISetMask(mask, XI_ButtonPress);
	XISetMask(mask, XI_Motion);
	XISetMask(mask, XI_KeyPress);

	// select on the window
	XISelectEvents(display, window, &eventmask, 1);
*/




    free( evenths );
    evenths = (xcb_event_handlers_t *) malloc( sizeof( xcb_event_handlers_t ) );
    xcb_event_handlers_init( connection, evenths );

	//cb_event_set_key_press_handler(evenths, )

#define SETUP_XCB_EVENT_EMITTER( event, handler )\
  xcb_event_set_##event##_handler( evenths, Handle##handler, this )
  SETUP_XCB_EVENT_EMITTER( motion_notify, MotionNotify );
  SETUP_XCB_EVENT_EMITTER( key_press, KeyPress );
#undef SETUP_XCB_EVENT_EMITTER


}

int MediaLayer::HandleMotionNotify( void *data, xcb_connection_t * __UNUSED__, xcb_motion_notify_event_t *event ) {
    Camera::Instance().setMouseLook(event->event_x - 400, event->event_y - 300);
}

int MediaLayer::HandleKeyPress( void *data, xcb_connection_t * __UNUSED__, xcb_key_press_event_t *event ) {
    //Camera::Instance().setMouseLook(event->event_x - 400, event->event_y - 300);
	cout << event->detail << "\n";
	//this->getKey(event->detail);

    xcb_key_symbols_t *syms;
	syms = xcb_key_symbols_alloc(__UNUSED__);
	xcb_keysym_t myass = xcb_key_symbols_get_keysym (syms, event->detail,0);
	cout << xcb_is_keypad_key(myass) << "\n";


}

void MediaLayer::getKey(xcb_keycode_t key){

}

void MediaLayer::createWindow(){






	     /* Create window */
	     eventmask =
	        		    //XCB_EVENT_MASK_EXPOSURE
	        		   	// |
	        		   	XCB_EVENT_MASK_KEY_PRESS
						| XCB_EVENT_MASK_POINTER_MOTION
						| XCB_EVENT_MASK_KEY_RELEASE
						/*| XCB_EVENT_MASK_BUTTON_PRESS
						| XCB_EVENT_MASK_BUTTON_RELEASE
						| XCB_EVENT_MASK_ENTER_WINDOW
						| XCB_EVENT_MASK_LEAVE_WINDOW

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

	           glxwindow =
	               glXCreateWindow(
	                   display,
	                   fb_config,
	                   window,
	                   0
	                   );

	           if(!window)
	        	   error("glXDestroyContext failed\n");

	           drawable = glxwindow;



	           /* make OpenGL context current */
	           if(!glXMakeContextCurrent(display, drawable, drawable, context))
	        	   error("glXMakeContextCurrent failed\n");

	           const static uint32_t values[] = { XCB_STACK_MODE_ABOVE };



	           /* Move the window on the top of the stack */
	           xcb_configure_window (connection, window, XCB_CONFIG_WINDOW_STACK_MODE, values);
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

void MediaLayer::eventLoop(){
   	//XI2
	//XEvent myEvent;
   	//XNextEvent(display, &myEvent);

	/*
	if (myEvent.xcookie.type == GenericEvent &&
			myEvent.xcookie.extension == opcode &&
	    XGetEventData(display, &myEvent.xcookie))
	{
	    switch(myEvent.xcookie.evtype)
	    {
	        case XI_ButtonPress:
	        case XI_Motion:
	        case XI_KeyPress:
	            cout << myEvent.xcookie.data << "\n";
	            break;
	    }
	}
	XFreeEventData(display, &myEvent.xcookie);
	*/

	xcb_event_poll_for_event_loop(evenths);
	//cout << xcb_event_handle (evenths, event) << "\n";

}

void MediaLayer::eventLoop2(){




	//event = xcb_wait_for_event (connection);



	while (event = xcb_poll_for_event (connection)) {

    switch (event->response_type & ~0x80) {
/*
        case XCB_EXPOSE:
            expose = (xcb_expose_event_t *)event;

            printf ("Window %d exposed. Region to be redrawn at location (%d,%d), with dimension (%d,%d)\n",
                    expose->window, expose->x, expose->y, expose->width, expose->height );
            break;
*/
/*
        case XCB_BUTTON_PRESS:

            bp = (xcb_button_press_event_t *)event;
            print_modifiers (bp->state);


            switch (bp->detail) {
            case 4:
                printf ("Wheel Button up in window %d, at coordinates (%d,%d)\n",
                        bp->event, bp->event_x, bp->event_y );
                break;
            case 5:
                printf ("Wheel Button down in window %d, at coordinates (%d,%d)\n",
                        bp->event, bp->event_x, bp->event_y );
                break;
            default:
                printf ("Button %d pressed in window %d, at coordinates (%d,%d)\n",
                        bp->detail, bp->event, bp->event_x, bp->event_y );
                break;
            }
            break;
        case XCB_BUTTON_RELEASE:
            br = (xcb_button_release_event_t *)event;
            print_modifiers(br->state);

            printf ("Button %d released in window %d, at coordinates (%d,%d)\n",
                    br->detail, br->event, br->event_x, br->event_y );
            break;
*/

        case XCB_MOTION_NOTIFY:
            motion = (xcb_motion_notify_event_t *)event;

            //printf ("Mouse moved in window %ld, at coordinates (%d,%d)\n",
            //        motion->event, motion->event_x, motion->event_y );
            relX = motion->event_x - 400;
            //mouseLastX = motion->event_x;

            relY = motion->event_y - 300;
            //mouseLastY = motion->event_y;

            //cout << relX << " " << relY << "\n";

            Camera::Instance().setMouseLook(relX, relY);
            if (grab) XWarpPointer(display, None, window, motion->event_x, motion->event_y, 800, 600, 400, 300);
            //printf ("Mouse moved at coordinates (%d,%d)\n",relX, relY );
            break;


        case XCB_KEY_PRESS:
        case XCB_KEY_RELEASE:
        	/*

        	 */
            kp = (xcb_key_press_event_t *)event;

            if(kp->detail == 9) quit = 1; //ESCAPE
            if(kp->detail == 25) Camera::Instance().forward(); //w
            if(kp->detail == 38) Camera::Instance().left(); //a
            if(kp->detail == 39) Camera::Instance().backward(); //s
            if(kp->detail == 40) Camera::Instance().right(); //d
            if(kp->detail == 58){ //m
				if (!grab){
					XGrabPointer(display, window, True, ButtonPressMask,
						GrabModeAsync, GrabModeAsync, window, None, CurrentTime);
					grab = true;
				}else{
					XUngrabPointer(display, CurrentTime);
					grab = false;
				}
            }
            /*
            printf("Key type %d\n", kp->detail);
            print_modifiers(kp->state);
            */
            break;
/*

            kr = (xcb_key_release_event_t *)event;
            print_modifiers(kr->state);
            printf ("Key released in window %d\n", kr->event);
            break;
*/
        default:
            /* Unknown event type, ignore it */
            //printf ("Unknown event: %d\n", event->response_type);
            break;
    }

    free (event);

	}
}

void MediaLayer::renderLoop(){
    while (!quit) {
    	eventLoop();
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
