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

MediaLayer::MediaLayer() {

	//FPS Stuff
	/*
	fps_lasttime = 0; //the last recorded time.
	fps_frames = 0; //frames passed since the last recorded fps.
	 */

	fullscreen = false;
	grab = false;

	quit = false;
	input = new Input();



}

void MediaLayer::init(string title) {
	programTile = title;
	//sf::RenderWindow window(sf::VideoMode(800, 600), title);
   
    sf::ContextSettings settings;
    settings.MajorVersion = LIBLUB_GL_MAJOR_VERION;
    settings.MinorVersion = LIBLUB_GL_MINOR_VERION;
/*
    settings.DepthBits         = 24; // Request a 24 bits depth buffer
    settings.StencilBits       = 8;  // Request a 8 bits stencil buffer
    settings.AntialiasingLevel = 2;  // Request 2 levels of antialiasing
*/

    window = new sf::RenderWindow(sf::VideoMode::GetFullscreenModes().at(0), title, sf::Style::Fullscreen, settings);

}

MediaLayer::~MediaLayer() {
   
}

/* A simple function that prints a message, the error code returned by SDL, and quits the application */
void MediaLayer::error(string msg)
{
    printf("%s: \n", msg.c_str());
    exit(1);
}

void MediaLayer::swapBuffers(){
    window->Display();
}

void MediaLayer::toggleFullScreen(){
	if(fullscreen){
		printf("Fullscreen Off\n");

		fullscreen = false;
	}else{
		printf("Fullscreen On\n");

		fullscreen = true;
	}

}

void MediaLayer::toggleMouseGrab(){
	if (grab){

		cout << "Grab Off\n";
		grab = false;
	}else{

		cout << "Grab On\n";
		grab = true;
	}
}

void MediaLayer::renderLoop(){
    while (!quit) {
        /* process pending events */
    	input->eventLoop();

    	/* update the screen */
        RenderEngine::Instance().display();

        // Draw some text on top of our OpenGL object
      /*
        window.SaveGLStates();
        sf::Text text(programTile);
        text.SetPosition(250.f, 450.f);
        text.SetColor(sf::Color(255, 255, 255, 170));
        window.Draw(text);
        window.RestoreGLStates();
*/
        swapBuffers();


    }
}
