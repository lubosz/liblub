/*
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 *      Author: bmonkey
 */

#include "SFMLInput.h"
#include "SFMLMediaLayer.h"

SFMLInput::SFMLInput(SFMLWindow * mediaLayer) {
  this->mediaLayer = mediaLayer;
}

SFMLInput::~SFMLInput() {
	// TODO Auto-generated destructor stub
}

void SFMLInput::eventLoop(){
    // Process events
//    sf::Event event;
//    while (mediaLayer->window->GetEvent(event))
//    {
//        // Close window : exit
//        if (event.Type == sf::Event::Closed)
//          mediaLayer->quit = true;
//
//        // Escape key : exit
//        if ((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Escape))
//          mediaLayer->quit = true;
/*
        // Adjust the viewport when the window is resized
        if (event.Type == sf::Event::Resized)
            glViewport(0, 0, event.Size.Width, event.Size.Height);
*/
//    }
}
