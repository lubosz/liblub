/*
 * MediaLayer.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include <iostream>
#include <sstream>
#include "MediaLayer.h"
#include "Camera.h"

MediaLayer::MediaLayer(string title, unsigned width, unsigned height) {
	//FPS Stuff
	fps_lasttime = 0; //the last recorded time.
	fps_frames = 0; //frames passed since the last recorded fps.
	programTile = title;

	quit = false;
    /* Create our window, opengl context, etc... */

    if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
        sdlDie("Unable to initialize SDL"); /* Or die on error */

    /* Request an opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, LIBLUB_GL_MAJOR_VERION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, LIBLUB_GL_MINOR_VERION);

    /* Enable double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Enable multisampling for a nice antialiased effect */
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    /* Create our window centered at 512x512 resolution */
    mainWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

            case SDLK_RIGHT:
                SceneGraph::Instance().lightPosition.at(0) += 1.0;
                SceneGraph::Instance().updateLight();
                break;

            case SDLK_LEFT:
            	SceneGraph::Instance().lightPosition.at(0) -= 1.0;
                SceneGraph::Instance().updateLight();
                break;

            case SDLK_UP:
                SceneGraph::Instance().lightPosition.at(1) += 1.0;
                SceneGraph::Instance().updateLight();
                break;

            case SDLK_DOWN:
            	SceneGraph::Instance().lightPosition.at(1) -= 1.0;
                SceneGraph::Instance().updateLight();
                break;
                /*
            case SDLK_KP_PLUS:
                break;
            case SDLK_KP_MINUS:
                break;
                 */
            case 'w':
            	//cout << "up\n";
            	Camera::Instance().move(0,0,.1);
            	break;

            case 's':
            	Camera::Instance().move(0,0,-.1);
            	//cout << "down\n";
            	break;

            case 'a':
            	Camera::Instance().move(.1,0,0);
            	//cout << "left\n";
            	break;

            case 'd':
            	Camera::Instance().move(-.1,0,0);
            	//cout << "rigt\n";
            	break;

            case 'r':
            	cout << "Reloading Shaders\n";
            	//RenderEngine::Instance().shaderProgram->reload();
            	break;

            default:
            	//cout << "ScanCode:\t" << event.key.keysym.scancode << "\n";
            	//cout << "sym:\t" << event.key.keysym.sym << "\n";
                 //no default key processing
                 //(stops compiler warnings for unhandled SDL keydefs
                 break;
            }
        break;

        case SDL_MOUSEMOTION:
        	Camera::Instance().rotate(event.motion.yrel,0,0);
        	Camera::Instance().rotate(0,event.motion.xrel,0);
            //pitch += event.motion.yrel;
            //if (pitch < -70) pitch = -70;
            //if (pitch > 70) pitch = 70;
            break;

         case SDL_QUIT:
            quit = true;
            break;

         case SDL_MOUSEBUTTONDOWN:
        	 switch( event.button.button ){
				 case SDL_BUTTON_WHEELUP:
			        Camera::Instance().move(0,0,.3);
					 break;
				 case SDL_BUTTON_WHEELDOWN:
					 Camera::Instance().move(0,0,-.3);
					 break;
				 default:
		        	 //cout << "Some win event!\n";
					 break;
        	 }

         case SDL_WINDOWEVENT: //User resized window

        	 switch( event.window.event ){
				 case SDL_WINDOWEVENT_RESIZED:


		        	 //cout << "Oldaspect:"<< Camera::Instance().aspect << "\n";
		        	 //cout << "w: " << event.resize.w << "\nh: " << event.resize.h << "\n";
		        	 //cout << "aspect:"<< (float(event.resize.w) / float(event.resize.h)) << "\n";

					 //Camera::Instance().setAspect(GLfloat(event.resize.w) / GLfloat(event.resize.h));
					 break;

				 default:
		        	 //cout << "Some win event!\n";
					 break;
        	 }
        	 //mainContext = SDL_GL_CreateContext(mainWindow);
        	 ////mainWindow = SDL_CreateWindow("loL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        	//		 event.resize.w, event.resize.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        	 break; //Event handled, fetch next :)

         default:
        	 if (DEBUG) cout << "Event:" << event.type << "\n";
        	 break;
        }
    }
}

void MediaLayer::renderLoop(){
    while (!quit) {
        /* process pending events */
    	eventLoop();
    	/* update the screen */
    	RenderEngine::Instance().display();
        swapBuffers();

        //FPS Stuff
        fps_frames++;
        if (fps_lasttime < SDL_GetTicks() - 1000)
        {
           fps_lasttime = SDL_GetTicks();
           fps_current = fps_frames;
           fps_frames = 0;
        }
        stringstream windowTitle;
        windowTitle << programTile << " - FPS: " << fps_current;
        SDL_SetWindowTitle(mainWindow, windowTitle.str().c_str());

    }
}
