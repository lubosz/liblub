/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include "SDLMediaLayer.h"
#include <iostream>
#include <sstream>
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "System/Config.h"
#include "System/Timer.h"
#include "SDLInput.h"

using std::stringstream;

SDLMediaLayer::SDLMediaLayer() {

	//FPS Stuff
	fps_lasttime = 0; //the last recorded time.
	fps_frames = 0; //frames passed since the last recorded fps.

	fullscreen = false;
	grab = false;

	quit = false;
	input = (Input*) new SDLInput(this);
}

void SDLMediaLayer::init(string title) {
	programTile = title;

    /* Create our window, opengl context, etc... */

    if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
        LogFatal << "Unable to initialize SDL"; /* Or die on error */

    /* Request an opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    vector<int> glContext = Config::Instance().values<int>("GLcontext");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glContext[0]);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glContext[1]);

    /* Enable double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Enable multisampling for a nice antialiased effect */
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0,&mode);
	printf("  Mode:  %dx%d %dHz %d bpp\n", mode.w, mode.h, mode.refresh_rate, SDL_BITSPERPIXEL(mode.format));
	width = mode.w;
	height = mode.h;

    //cout << "WIDTH/HEIGHT "<< info->current_w << " " << info->current_h << "\n";
	LogDebug << "WIDTH/HEIGHT "<< width << " " << height << "\n";
	SceneData::Instance().getCurrentCamera()->setAspect(float(width)/ float(height));
    /* Create our window centered at 512x512 resolution */
    mainWindow = SDL_CreateWindow(
				title.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				width, height,
				SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
				//| SDL_WINDOW_FULLSCREEN
				//| SDL_WINDOW_RESIZABLE
			);

    if (!mainWindow) /* Die if creation failed */
    	LogError << "Unable to create window";

    /* Create our opengl context and attach it to our window */
    mainContext = SDL_GL_CreateContext(mainWindow);

    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(Config::Instance().value<int>("Vsync"));

    /* Enable Z depth testing so objects closest to the viewpoint are in front of objects further away */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //SDL_SetRelativeMouseMode(SDL_TRUE);

    /*
    SDL_GetDesktopDisplayMode(&fsmode);
    cout << fsmode.format << " " << fsmode.w << " " << fsmode.h << " " << fsmode.refresh_rate << " " << long(fsmode.driverdata) << endl;
    cout << "SDL_SetWindowDisplayMode = " << SDL_SetWindowDisplayMode(mainWindow, &fsmode) << endl;
    */

}

SDLMediaLayer::~SDLMediaLayer() {
    /* Delete our opengl context, destroy our window, and shutdown SDL */
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
}

void SDLMediaLayer::swapBuffers(){
    /* Swap our buffers to make our changes visible */
    SDL_GL_SwapWindow(mainWindow);

    /* Sleep for roughly 33 milliseconds between frames */
    //SDL_Delay(33);
}

void SDLMediaLayer::toggleFullScreen(){
	if(fullscreen){
		printf("Fullscreen Off\n");
		if(SDL_SetWindowFullscreen(mainWindow, SDL_FALSE)!=0){
			printf ("Unable to switch window to fullscreen mode:%s\n", SDL_GetError());
		}
		fullscreen = false;
	}else{
		printf("Fullscreen On\n");
		if(SDL_SetWindowFullscreen(mainWindow, SDL_TRUE)!=0){
			printf ("Unable to switch window to fullscreen mode:%s\n", SDL_GetError());
		}
		fullscreen = true;
	}

}

void SDLMediaLayer::toggleMouseGrab(){
	if (grab){
		SDL_SetWindowGrab(mainWindow, SDL_FALSE);
//		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_ShowCursor(1);
		LogDebug << "Grab Off";
		grab = false;
	}else{
		SDL_SetWindowGrab(mainWindow, SDL_TRUE);
		//SDL_WM_GrabInput(SDL_GRAB_ON);
		SDL_ShowCursor(0);
		LogDebug << "Grab On";
		grab = true;
	}
}

void SDLMediaLayer::renderFrame(){
  Timer::Instance().frame();

//    while (!quit) {
        /* process pending events */
//    	input->eventLoop();
    	/* update the screen */
//    	RenderEngine::Instance().display();
        swapBuffers();

        if(grab){
        	SDL_WarpMouseInWindow(mainWindow, width/2, height/2);
        }

        stringstream windowTitle;
        windowTitle << "FPS: " << Timer::Instance().fps_current;
        SDL_SetWindowTitle(mainWindow, windowTitle.str().c_str());

//    }
}

void SDLMediaLayer::mouseLook(int x, int y) {

}
