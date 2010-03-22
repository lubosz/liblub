/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include <iostream>
#include "MediaLayer.h"

MediaLayer::MediaLayer(string title, unsigned width, unsigned height) {
	quit = false;
    /* Create our window, opengl context, etc... */

    if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
        sdlDie("Unable to initialize SDL"); /* Or die on error */

    /* Request an opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    /* Enable double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Enable multisampling for a nice antialiased effect */
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    /* Create our window centered at 512x512 resolution */
    mainWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!mainWindow) /* Die if creation failed */
        sdlDie("Unable to create window");

    /* Create our opengl context and attach it to our window */
    mainContext = SDL_GL_CreateContext(mainWindow);

    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);

    /* Enable Z depth testing so objects closest to the viewpoint are in front of objects further away */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

}

MediaLayer::~MediaLayer() {
    /* Delete our opengl context, destroy our window, and shutdown SDL */
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
}

/* A simple function that prints a message, the error code returned by SDL, and quits the application */
void MediaLayer::sdlDie(string msg)
{
    printf("%s: %s\n", msg.c_str(), SDL_GetError());
    SDL_Quit();
    exit(1);
}

void MediaLayer::swapBuffers(){
    /* Swap our buffers to make our changes visible */
    SDL_GL_SwapWindow(mainWindow);

    /* Sleep for roughly 33 milliseconds between frames */
    SDL_Delay(33);
}

void MediaLayer::eventLoop(){
    while( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
        case SDL_KEYDOWN:
            switch ( event.key.keysym.sym ) {
            case SDLK_ESCAPE:
            	quit = true;
				break;

            case SDLK_KP_PLUS:
                //level++;
                //if (level > 5) level=5;
                break;

            case SDLK_KP_MINUS:
                //level--;
                //if (level < 0) level=0;
                break;

            default:
                 //no default key processing
                 //(stops compiler warnings for unhandled SDL keydefs
                 break;
            }
        break;

        case SDL_MOUSEMOTION:
            //pitch += event.motion.yrel;
            //if (pitch < -70) pitch = -70;
            //if (pitch > 70) pitch = 70;
            break;

         case SDL_QUIT:
            quit = true;
            break;
        }
    }
}

void MediaLayer::renderLoop(RenderEngine * renderEngine){
    while (!quit) {
        /* process pending events */
    	eventLoop();
    	/* update the screen */
    	renderEngine->display();
        swapBuffers();

    }
}
