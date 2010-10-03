/*
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#include "Input.h"
#include "Camera.h"
#include "MediaLayer.h"
#include "RenderEngine.h"

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

    while( SDL_PollEvent( &event ) ) {



        switch( event.type ) {
        /*
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_K
			*/

			//break;
        	/*

            switch ( event.key.keysym.sym ) {
            case SDLK_ESCAPE:

				break;

            case SDLK_RIGHT:
                SceneGraph::Instance().lightPosition += QVector3D(0.1,0,0);
                SceneGraph::Instance().updateLight();
                break;

            case SDLK_LEFT:
            	SceneGraph::Instance().lightPosition += QVector3D(-0.1,0,0);
                SceneGraph::Instance().updateLight();
                break;

            case SDLK_UP:
                SceneGraph::Instance().lightPosition += QVector3D(0,0.1,0);
                SceneGraph::Instance().updateLight();
                break;

            case SDLK_DOWN:
            	SceneGraph::Instance().lightPosition += QVector3D(0,-0.1,0);
                SceneGraph::Instance().updateLight();
                break;

            case '1':
                SceneGraph::Instance().lightPosition += QVector3D(0,0,0.1);
                SceneGraph::Instance().updateLight();
                break;

            case '7':
            	SceneGraph::Instance().lightPosition += QVector3D(0,0,-0.1);
                SceneGraph::Instance().updateLight();
                break;

            case SDLK_KP_PLUS:
                break;
            case SDLK_KP_MINUS:
                break;

            case 'w':
            	Camera::Instance().forward();
            	break;

            case 's':
            	Camera::Instance().backward();
            	break;

            case 'a':
            	Camera::Instance().left();
            	break;

            case 'd':
            	Camera::Instance().right();
            	break;

            case 'r':
            	cout << "Reloading Shaders\n";
            	//RenderEngine::Instance().shaderProgram->reload();
            	break;

            case 'f':
            	toggleFullScreen();
            	break;

            case 't':
            	RenderEngine::Instance().toggleFBO();
            	break;

            case 'g':
            	if (grab){
            		SDL_SetWindowGrab(mainWindow,0);
            		//SDL_WM_GrabInput(SDL_GRAB_OFF);
            		SDL_ShowCursor(1);
            		cout << "Grab Off\n";
            		grab = false;
            	}else{
            		SDL_SetWindowGrab(mainWindow,1);
            		//SDL_WM_GrabInput(SDL_GRAB_ON);
            		SDL_ShowCursor(0);
            		cout << "Grab On\n";
            		grab = true;
            	}
            	break;

            default:
                 break;
            }

        break;
 */
        case SDL_MOUSEMOTION:
        	Camera::Instance().setMouseLook(event.motion.xrel, event.motion.yrel);
        	//Camera::Instance().setMouseLookInverseVP(event.motion.x, event.motion.y);
            break;

         case SDL_QUIT:
        	MediaLayer::Instance().shutdown();
            break;

         case SDL_MOUSEWHEEL:
        	 cout << "Wheel\n";
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
        	 if (DEBUG) cout << "Event:" << event.type << "\n";
        	 break;
        }
    }
}
