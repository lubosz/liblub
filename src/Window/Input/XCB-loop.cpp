/*
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#include "Input.h"
#include "Camera.h"
#include "MediaLayer.h"

Input::Input(xcb_connection_t *connection) {
	this->connection = connection;
}

Input::~Input() {
	// TODO Auto-generated destructor stub
}

void Input::eventLoop(){

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
            Camera::Instance().setMouseLook(motion->event_x - 400, motion->event_y - 300);
            //if (grab) XWarpPointer(display, None, window, motion->event_x, motion->event_y, 800, 600, 400, 300);
            //printf ("Mouse moved at coordinates (%d,%d)\n",relX, relY );
            break;


        case XCB_KEY_PRESS:
        case XCB_KEY_RELEASE:
            kp = (xcb_key_press_event_t *)event;

            if(kp->detail == 9) MediaLayer::Instance().shutdown(); //ESCAPE
            if(kp->detail == 25) Camera::Instance().forward(); //w
            if(kp->detail == 38) Camera::Instance().left(); //a
            if(kp->detail == 39) Camera::Instance().backward(); //s
            if(kp->detail == 40) Camera::Instance().right(); //d
            /*
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
            */
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
