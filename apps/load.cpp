#include "Application.h"
#include "SceneLoader.h"

class LoadApp: public Application {

public:


	LoadApp(string sceneName) {

//		if(sceneXML.tagName() == "Scene")
//			programName = sceneXML.attribute("name").toStdString();

		programName ="XML Loader";
		sceneLoader = new SceneLoader("media/scenes/"+ sceneName + ".xml");
	}



	void scene() {
		sceneLoader->load();

	}
};

int main(int argc, char *argv[]) {
	LoadApp(argv[0]).run();
}
