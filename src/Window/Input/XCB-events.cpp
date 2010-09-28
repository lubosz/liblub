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
    evenths = (xcb_event_handlers_t *) malloc( sizeof( xcb_event_handlers_t ) );
    xcb_event_handlers_init( connection, evenths );

	//cb_event_set_key_press_handler(evenths, )

#define SETUP_XCB_EVENT_EMITTER( event, handler )\
  xcb_event_set_##event##_handler( evenths, Handle##handler, this )
  SETUP_XCB_EVENT_EMITTER( motion_notify, MotionNotify );
  SETUP_XCB_EVENT_EMITTER( key_press, KeyPress );
#undef SETUP_XCB_EVENT_EMITTER
}

Input::~Input() {
	free( evenths );
}

void Input::eventLoop(){
	xcb_event_poll_for_event_loop(evenths);
}

int Input::HandleMotionNotify( void *data, xcb_connection_t * __UNUSED__, xcb_motion_notify_event_t *event ) {
    Camera::Instance().setMouseLook(event->event_x - 400, event->event_y - 300);
}

int Input::HandleKeyPress( void *data, xcb_connection_t * __UNUSED__, xcb_key_press_event_t *event ) {
	//cout << event->detail << "\n";

    xcb_key_symbols_t *syms;
	syms = xcb_key_symbols_alloc(__UNUSED__);
	xcb_keysym_t pressedKey = xcb_key_symbols_get_keysym (syms, event->detail,0);

	if (pressedKey == XK_Escape) MediaLayer::Instance().shutdown();

	if (pressedKey == XK_w)  Camera::Instance().forward();
	if (pressedKey == XK_a)  Camera::Instance().left();
	if (pressedKey == XK_s)  Camera::Instance().backward();
	if (pressedKey == XK_d)  Camera::Instance().right();
}
