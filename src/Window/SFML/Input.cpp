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
    // Process events
    sf::Event event;
    while (MediaLayer::Instance().window->GetEvent(event))
    {
        // Close window : exit
        if (event.Type == sf::Event::Closed)
        	MediaLayer::Instance().window->Close();

        // Escape key : exit
        if ((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Escape))
        	MediaLayer::Instance().window->Close();
/*
        // Adjust the viewport when the window is resized
        if (event.Type == sf::Event::Resized)
            glViewport(0, 0, event.Size.Width, event.Size.Height);
*/
    }
}
