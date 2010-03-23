#include <boost/multi_array.hpp>

#include "MediaLayer.h"
#include "RenderEngine.h"

#define PROGRAM_NAME "Cone Step Mapping"

MediaLayer * mediaLayer;
RenderEngine * renderEngine;

int main(int argc, char *argv[])
{

	mediaLayer = new MediaLayer(PROGRAM_NAME, 750, 750);

	renderEngine = new RenderEngine();
	mediaLayer->renderLoop(renderEngine);

    delete renderEngine;
    delete mediaLayer;

}
