/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */
#include "MediaLayer.h"

#include <iostream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>

#include "Camera.h"

#include <xcb/xcb_atom.h>


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

     /* Open Xlib Display */
     display = XOpenDisplay(0);
     if(!display) error("Can't open display\n");

     default_screen = DefaultScreen(display);

     /* Get the XCB connection from the display */
     connection = XGetXCBConnection(display);
     if(!connection){
         XCloseDisplay(display);
         error("Can't get xcb connection from display\n");
     }

     /* Acquire event queue ownership */
     XSetEventQueueOwner(display, XCBOwnsEventQueue);

     /* Find XCB screen */
     xcb_screen_t *screen = 0;
     xcb_screen_iterator_t screen_iter =
         xcb_setup_roots_iterator(xcb_get_setup(connection));
     for(int screen_num = default_screen;
         screen_iter.rem && screen_num > 0;
         --screen_num, xcb_screen_next(&screen_iter));
     screen = screen_iter.data;

     /* Initialize window and OpenGL context, run main loop and deinitialize */
     int visualID = 0;

     /* Query framebuffer configurations */
           GLXFBConfig *fb_configs = 0;
           int num_fb_configs = 0;
           fb_configs = glXGetFBConfigs(display, default_screen, &num_fb_configs);
           if(!fb_configs || num_fb_configs == 0){
        	   error("glXGetFBConfigs failed\n");
           }

           /* Select first framebuffer config and query visualID */
           GLXFBConfig fb_config = fb_configs[0];
           glXGetFBConfigAttrib(display, fb_config, GLX_VISUAL_ID , &visualID);

           /* Create OpenGL context */
           context = glXCreateNewContext(display, fb_config, GLX_RGBA_TYPE, 0, True);
           if(!context)
           {
        	   error("glXCreateNewContext failed\n");
           }

           /* Create XID's for colormap and window */
           xcb_colormap_t colormap = xcb_generate_id(connection);
           window = xcb_generate_id(connection);

           /* Create colormap */
           xcb_create_colormap(
               connection,
               XCB_COLORMAP_ALLOC_NONE,
               colormap,
               screen->root,
               visualID
               );

           /* Create window */
           uint32_t eventmask = XCB_EVENT_MASK_EXPOSURE
        		   	| XCB_EVENT_MASK_KEY_PRESS
					| XCB_EVENT_MASK_POINTER_MOTION
					| XCB_EVENT_MASK_BUTTON_PRESS
					| XCB_EVENT_MASK_BUTTON_RELEASE
					| XCB_EVENT_MASK_ENTER_WINDOW
					| XCB_EVENT_MASK_LEAVE_WINDOW
					| XCB_EVENT_MASK_KEY_RELEASE;
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


           /* set the title of the window */
           xcb_change_property (connection,
                                XCB_PROP_MODE_REPLACE,
                                window,
                                WM_NAME,
                                STRING,
                                8,
                                strlen (programTile.c_str()),
                                programTile.c_str() );


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
           {
               xcb_destroy_window(connection, window);
               glXDestroyContext(display, context);

               error("glXDestroyContext failed\n");
           }

           drawable = glxwindow;

           /* make OpenGL context current */
           if(!glXMakeContextCurrent(display, drawable, drawable, context))
           {
               xcb_destroy_window(connection, window);
               glXDestroyContext(display, context);

               error("glXMakeContextCurrent failed\n");
           }

           glEnable(GL_DEPTH_TEST);
           glDepthFunc(GL_LESS);


           const static uint32_t values[] = { XCB_STACK_MODE_ABOVE };

           /* Move the window on the top of the stack */
           xcb_configure_window (connection, window, XCB_CONFIG_WINDOW_STACK_MODE, values);


}

MediaLayer::~MediaLayer() {
    /* Cleanup */
    glXDestroyWindow(display, glxwindow);
    xcb_destroy_window(connection, window);
    glXDestroyContext(display, context);
    XCloseDisplay(display);
}

/* A simple function that prints a message, the error code returned by SDL, and quits the application */
void MediaLayer::error(string msg)
{
	fprintf(stderr, msg.c_str());
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

/* print names of modifiers present in mask */
void print_modifiers (uint32_t mask)
{
    char *MODIFIERS[] = {
            "Shift", "Lock", "Ctrl", "Alt",
            "Mod2", "Mod3", "Mod4", "Mod5",
            "Button1", "Button2", "Button3", "Button4", "Button5"
    };

    printf ("Modifier mask: ");
    for (char **modifier = MODIFIERS ; mask; mask >>= 1, ++modifier) {
        if (mask & 1) {
            printf (*modifier);
        }
    }
    printf ("\n");
}

void MediaLayer::eventLoop(){

    xcb_generic_event_t *event;
    xcb_expose_event_t *expose;
    xcb_button_press_event_t *bp;
    xcb_button_release_event_t *br;
    xcb_motion_notify_event_t *motion;
    xcb_enter_notify_event_t *enter;
	xcb_leave_notify_event_t *leave;
	xcb_key_press_event_t *kp;
	xcb_key_release_event_t *kr;

	int relX, relY;

    //while (event = xcb_wait_for_event (connection)) {
	event = xcb_wait_for_event (connection);
        switch (event->response_type & ~0x80) {
        case XCB_EXPOSE:
            expose = (xcb_expose_event_t *)event;

            printf ("Window %ld exposed. Region to be redrawn at location (%d,%d), with dimension (%d,%d)\n",
                    expose->window, expose->x, expose->y, expose->width, expose->height );
            break;

        case XCB_BUTTON_PRESS:
            bp = (xcb_button_press_event_t *)event;
            print_modifiers (bp->state);

            switch (bp->detail) {
            case 4:
                printf ("Wheel Button up in window %ld, at coordinates (%d,%d)\n",
                        bp->event, bp->event_x, bp->event_y );
                break;
            case 5:
                printf ("Wheel Button down in window %ld, at coordinates (%d,%d)\n",
                        bp->event, bp->event_x, bp->event_y );
                break;
            default:
                printf ("Button %d pressed in window %ld, at coordinates (%d,%d)\n",
                        bp->detail, bp->event, bp->event_x, bp->event_y );
                break;
            }
            break;
        case XCB_BUTTON_RELEASE:
            br = (xcb_button_release_event_t *)event;
            print_modifiers(br->state);

            printf ("Button %d released in window %ld, at coordinates (%d,%d)\n",
                    br->detail, br->event, br->event_x, br->event_y );
            break;

        case XCB_MOTION_NOTIFY:
            motion = (xcb_motion_notify_event_t *)event;

            //printf ("Mouse moved in window %ld, at coordinates (%d,%d)\n",
            //        motion->event, motion->event_x, motion->event_y );
            relX = motion->event_x - mouseLastX;
            mouseLastX = motion->event_x;

            relY = motion->event_y - mouseLastY;
            mouseLastY = motion->event_y;

            Camera::Instance().setMouseLook(relX, relY);
            //printf ("Mouse moved at coordinates (%d,%d)\n",relX, relY );
            break;

        case XCB_ENTER_NOTIFY:
            enter = (xcb_enter_notify_event_t *)event;

            printf ("Mouse entered window %ld, at coordinates (%d,%d)\n",
                    enter->event, enter->event_x, enter->event_y );
            break;

        case XCB_LEAVE_NOTIFY:
            leave = (xcb_leave_notify_event_t *)event;

            printf ("Mouse left window %ld, at coordinates (%d,%d)\n",
                    leave->event, leave->event_x, leave->event_y );
            break;

        case XCB_KEY_PRESS:
            kp = (xcb_key_press_event_t *)event;
            print_modifiers(kp->state);

            if(kp->event == XK_Escape) quit = 1;

            printf ("Key pressed in window %ld\n",
                    kp->event);
            break;

        case XCB_KEY_RELEASE:
            kr = (xcb_key_release_event_t *)event;
            print_modifiers(kr->state);
            printf ("Key released in window %ld\n",
                    kr->event);
            break;

        default:
            /* Unknown event type, ignore it */
            printf ("Unknown event: %d\n",
                    event->response_type);
            break;
        }

        free (event);
    //}
}

void MediaLayer::renderLoop(){
    while (!quit) {
    	eventLoop();
    	RenderEngine::Instance().display();
        swapBuffers();

        struct timespec now;
        static struct timespec start;
        clock_gettime(CLOCK_MONOTONIC, &now);
        unsigned ticks = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;

        //FPS Stuff
        fps_frames++;
        if (fps_lasttime < ticks - 1000)
        {
           fps_lasttime = ticks;
           fps_current = fps_frames;
           fps_frames = 0;
        }
        stringstream windowTitle;
        windowTitle << programTile << " - FPS: " << fps_current;

        xcb_change_property (connection,
                             XCB_PROP_MODE_REPLACE,
                             window,
                             WM_NAME,
                             STRING,
                             8,
                             strlen ( windowTitle.str().c_str()),
                             windowTitle.str().c_str() );
        //SDL_SetWindowTitle(mainWindow, windowTitle.str().c_str());
    }
}
