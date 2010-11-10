/*
 * Input.h
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#pragma once

#include <list>

using namespace std;


#ifdef XCBEVENTS
	#include <xcb/xcb_event.h>
	#include <xcb/xcb_keysyms.h>
#endif


//XCBLOOP
#ifdef WITH_XCB
	#include <xcb/xcb_keysyms.h>
#endif

#ifdef WITH_SDL
	#define SDL_NO_COMPAT
	#include "SDL.h"
	#include "SDL_compat.h"
#endif




class Input {
	public:

		#ifdef WITH_SDL
			Input();
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

		#ifdef WITH_XCB
			xcb_connection_t *connection;

			xcb_key_symbols_t *syms;
			xcb_keysym_t pressedKey;

			list<xcb_keysym_t> pressedKeys;

			void checkKey(xcb_keysym_t pressedKey);
		#endif

#ifdef WITH_SDL
		Uint8 *keystate;
		SDL_Event event;
#endif
};
