/*
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#include "Camera.h"
#include "MediaLayer.h"

Input::Input(xcb_connection_t *connection) {
	this->connection = connection;
	syms = xcb_key_symbols_alloc(connection);
	pressedKeys = list<xcb_keysym_t>();
}

Input::~Input() {
	// TODO Auto-generated destructor stub
}

void Input::eventLoop(){
	xcb_keysym_t pressedKey;

	xcb_generic_event_t *event;

	xcb_expose_event_t *expose;
	xcb_button_press_event_t *bp;
	xcb_button_release_event_t *br;
	xcb_motion_notify_event_t *motion;
	xcb_enter_notify_event_t *enter;
	xcb_leave_notify_event_t *leave;
	xcb_key_press_event_t *kp;
	xcb_key_release_event_t *kr;

	while (event = xcb_poll_for_event (connection)) {

    switch (event->response_type & ~0x80) {
    	case XCB_CONFIGURE_REQUEST:
    		cout << "Reconfigure!!\n";
    		/*
    			uint32_t c[6],*cp=c;
    			if(((xcb_configure_request_event_t*)p)->value_mask&XCB_CONFIG_WINDOW_X)*cp++=((xcb_configure_request_event_t*)p)->x;
    			if(((xcb_configure_request_event_t*)p)->value_mask&XCB_CONFIG_WINDOW_Y)*cp++=((xcb_configure_request_event_t*)p)->y;
    			if(((xcb_configure_request_event_t*)p)->value_mask&XCB_CONFIG_WINDOW_WIDTH)*cp++=((xcb_configure_request_event_t*)p)->width;
    			if(((xcb_configure_request_event_t*)p)->value_mask&XCB_CONFIG_WINDOW_HEIGHT)*cp++=((xcb_configure_request_event_t*)p)->height;
    			if(((xcb_configure_request_event_t*)p)->value_mask&XCB_CONFIG_WINDOW_SIBLING)*cp++=((xcb_configure_request_event_t*)p)->sibling;
    			if(((xcb_configure_request_event_t*)p)->value_mask&XCB_CONFIG_WINDOW_STACK_MODE)*cp=((xcb_configure_request_event_t*)p)->stack_mode;
    			xcb_configure_window(dpy,((xcb_configure_request_event_t*)p)->window,((xcb_configure_request_event_t*)p)->value_mask&~XCB_CONFIG_WINDOW_BORDER_WIDTH,c);
    			for(;x>-1;x--)
    				if(cs[x]==((xcb_configure_request_event_t*)p)->window&&((xcb_configure_request_event_t*)p)->value_mask&XCB_CONFIG_WINDOW_STACK_MODE)
    					switch(((xcb_configure_request_event_t*)p)->stack_mode){
    					case XCB_STACK_MODE_BELOW:y=0;
    					case XCB_STACK_MODE_ABOVE:goto stack;
    					}
    					*/
    	break;
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
            MediaLayer::Instance().mouseLook(motion->event_x, motion->event_y);
            break;

        //case XCB_KEYMAP_NOTIFY:

        case XCB_KEY_RELEASE:

            kr = (xcb_key_release_event_t *)event;
            pressedKey = xcb_key_symbols_get_keysym (syms, kr->detail,0);
            pressedKeys.remove(pressedKey);

            break;

        case XCB_KEY_PRESS:
            kp = (xcb_key_press_event_t *)event;
            pressedKey = xcb_key_symbols_get_keysym (syms, kp->detail,0);

            switch (pressedKey){
				case XK_Escape:
					MediaLayer::Instance().shutdown();
					break;
				case XK_m:
					MediaLayer::Instance().toggleMouseGrab();
					break;
				case XK_f:
					MediaLayer::Instance().toggleFullScreen();
					break;
				case XK_p:
					RenderEngine::Instance().toggleFBO();
					break;
				case XK_c:
					RenderEngine::Instance().toggleLightView();
					break;
				default:
					pressedKeys.push_back(pressedKey);
            }
            break;

        default:
            /* Unknown event type, ignore it */
            //printf ("Unknown event: %d\n", event->response_type);
            break;
    }

    	free (event);

	}

	foreach( xcb_keysym_t key, pressedKeys )
    {
    	checkKey(key);
    }
}

void Input::checkKey(xcb_keysym_t pressedKey){
    switch (pressedKey){
		case XK_w:
			Camera::Instance().forward();
			break;
		case XK_a:
			 Camera::Instance().left();
			break;
		case XK_s:
			Camera::Instance().backward();
			break;
		case XK_d:
			Camera::Instance().right();
			break;


			//Light
			case XK_Left:
                SceneGraph::Instance().light->moveLeft();
                break;
			case XK_Right:
                SceneGraph::Instance().light->moveRight();
				break;
			case XK_Up:
                SceneGraph::Instance().light->moveUp();
                break;
			case XK_Down:
                SceneGraph::Instance().light->moveDown();
                break;
            case XK_1:
                SceneGraph::Instance().light->moveForward();
                break;
            case XK_7:
                SceneGraph::Instance().light->moveBack();
				break;
    }
}
