/*
 * Input.h
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"

#define XCBLOOP

#ifdef XCBEVENTS
	#include <xcb/xcb_event.h>
	#include <xcb/xcb_keysyms.h>
#endif

#ifdef XCBLOOP
	#include <xcb/xcb_keysyms.h>
	#include <list>
#endif

#ifdef XI2
#include <X11/extensions/XInput2.h>
#endif

class Input {
	public:

		#ifdef XI2
			Input(Display *display, Window window);
		#else
			Input(xcb_connection_t *connection);
		#endif

			virtual ~Input();
			void eventLoop();
	private:

		#ifdef XCBEVENTS

			xcb_event_handlers_t *evenths;

			#define HANDLER_DECLARE( event, name )\
			  static int Handle##name( void*, xcb_connection_t*, xcb_##event##_event_t* )

			  HANDLER_DECLARE( motion_notify, MotionNotify );
			  HANDLER_DECLARE( key_press, KeyPress );
			  HANDLER_DECLARE( key_release, KeyRelease );
			#undef HANDLER_DECLARE

		#endif

		#ifdef XCBLOOP
			xcb_connection_t *connection;

			xcb_key_symbols_t *syms;
			xcb_keysym_t pressedKey;

			list<xcb_keysym_t> pressedKeys;

			void checkKey(xcb_keysym_t pressedKey);
		#endif

		#ifdef XI2
			Display *display;
		    int opcode;
		#endif
};