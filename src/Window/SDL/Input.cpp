/*
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#include "Window/Input.h"
#include "Scene/Camera.h"
#include "Window/MediaLayer.h"
#include "Renderer/RenderEngine.h"
#include "System/Logger.h"

Input::Input() {

}

Input::~Input() {
	// TODO Auto-generated destructor stub
}

void Input::eventLoop(){
	keystate = SDL_GetKeyboardState(NULL);
	if ( keystate[SDL_SCANCODE_RETURN] ) printf("<RETURN> is pressed.\n");
	if ( keystate[SDL_SCANCODE_ESCAPE] ) MediaLayer::Instance().shutdown();
	if ( keystate[SDL_SCANCODE_W] ) Camera::Instance().forward();
	if ( keystate[SDL_SCANCODE_A] ) Camera::Instance().left();
	if ( keystate[SDL_SCANCODE_S] ) Camera::Instance().backward();
	if ( keystate[SDL_SCANCODE_D] ) Camera::Instance().right();

	if ( keystate[SDL_SCANCODE_LEFT] ) SceneGraph::Instance().light->moveLeft();
	if ( keystate[SDL_SCANCODE_RIGHT] ) SceneGraph::Instance().light->moveRight();
	if ( keystate[SDL_SCANCODE_UP] ) SceneGraph::Instance().light->moveUp();
	if ( keystate[SDL_SCANCODE_DOWN] ) SceneGraph::Instance().light->moveDown();
	if ( keystate[SDL_SCANCODE_1] ) SceneGraph::Instance().light->moveForward();
	if ( keystate[SDL_SCANCODE_7] ) SceneGraph::Instance().light->moveBack();

    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
			case SDL_KEYDOWN:
				switch ( event.key.keysym.sym ) {

						case 'f':
							MediaLayer::Instance().toggleFullScreen();
							break;

						case 't':
							RenderEngine::Instance().toggleFBO();
							break;

						case 'g':
							MediaLayer::Instance().toggleMouseGrab();
							break;

						default:
							 break;
				}

        case SDL_MOUSEMOTION:
        	Camera::Instance().setMouseLook(event.motion.xrel, event.motion.yrel);
        	//Camera::Instance().setMouseLookInverseVP(event.motion.x, event.motion.y);
            break;

         case SDL_QUIT:
        	MediaLayer::Instance().shutdown();
            break;

         case SDL_MOUSEWHEEL:
        	 Logger::Instance().log("DEBUG","Wheel");
        	 Camera::Instance().setMouseZoom(event.wheel.x,event.wheel.y);
        	 break;

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
//        	 if (DEBUG) cout << "Event:" << event.type << "\n";
        	 break;
        }
    }
}
