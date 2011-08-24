/*
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#include "Window/Input.h"
#include "Scene/SceneData.h"
#include "Window/MediaLayer.h"
#include "Renderer/RenderEngine.h"
#include "System/Logger.h"
#include "SDLInput.h"
#include "SDLMediaLayer.h"

SDLInput::SDLInput(SDLWindow * mediaLayer) {
  // TODO(bmonkey): Hardcoded values => xml
  inputSpeed = .01;
  mouseSensitivity = .1;
  this->mediaLayer = mediaLayer;
}

SDLInput::~SDLInput() {
	// TODO Auto-generated destructor stub
}

void SDLInput::eventLoop(){
	keystate = SDL_GetKeyboardState(NULL);
	if ( keystate[SDL_SCANCODE_RETURN] ) printf("<RETURN> is pressed.\n");
	if ( keystate[SDL_SCANCODE_ESCAPE] ) mediaLayer->shutdown();
	if ( keystate[SDL_SCANCODE_W] ) SceneData::Instance().getCurrentCamera()->forwardDirection(inputSpeed);
	if ( keystate[SDL_SCANCODE_A] ) SceneData::Instance().getCurrentCamera()->leftDirection(inputSpeed);
	if ( keystate[SDL_SCANCODE_S] ) SceneData::Instance().getCurrentCamera()->backwardDirection(inputSpeed);
	if ( keystate[SDL_SCANCODE_D] ) SceneData::Instance().getCurrentCamera()->rightDirection(inputSpeed);

//	if ( keystate[SDL_SCANCODE_LEFT] ) SceneGraph::Instance().light->moveLeft();
//	if ( keystate[SDL_SCANCODE_RIGHT] ) SceneGraph::Instance().light->moveRight();
//	if ( keystate[SDL_SCANCODE_UP] ) SceneGraph::Instance().light->moveUp();
//	if ( keystate[SDL_SCANCODE_DOWN] ) SceneGraph::Instance().light->moveDown();
//	if ( keystate[SDL_SCANCODE_1] ) SceneGraph::Instance().light->moveForward();
//	if ( keystate[SDL_SCANCODE_7] ) SceneGraph::Instance().light->moveBack();

    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
			case SDL_KEYDOWN:
				switch ( event.key.keysym.sym ) {

						case 'f':
						  mediaLayer->toggleFullScreen();
							break;

						case 't':
							RenderEngine::Instance().toggleFBO();
							break;

						case 'g':
						  mediaLayer->toggleMouseGrab();
							break;

						default:
							 break;
				}

        case SDL_MOUSEMOTION:
//        	SceneData::Instance().getCurrentCamera()->setMouseLook(event.motion.xrel, event.motion.yrel);
        	//SceneData::Instance().getCurrentCamera()->setMouseLookInverseVP(event.motion.x, event.motion.y);
          SceneData::Instance().getCurrentCamera()->setMouseLook(event.motion.xrel, event.motion.yrel, mouseSensitivity);
            break;

         case SDL_QUIT:
           mediaLayer->shutdown();
            break;

         case SDL_MOUSEWHEEL:
        	 LogDebug <<"Wheel";
        	 SceneData::Instance().getCurrentCamera()->setMouseZoom(event.wheel.x,event.wheel.y);
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
