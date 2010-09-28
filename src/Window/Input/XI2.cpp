/*
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#include "Input.h"
#include "MediaLayer.h"

Input::Input(Display *display, Window window) {
	this->display = display;

	/* XInput Extension available? */
	int event, myError;
	if (!XQueryExtension(display, "XInputExtension", &opcode, &event, &myError)) MediaLayer::Instance().error("X Input extension not available.");

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
}

Input::~Input() {
	// TODO Auto-generated destructor stub
}

void Input::eventLoop(){
   	//XI2
	XEvent myEvent;
   	XNextEvent(display, &myEvent); //TODO: Crashes here


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
}
