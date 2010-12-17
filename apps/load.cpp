#include "Application.h"
#include "SceneLoader.h"

class LoadApp: public Application {

public:


	LoadApp() {

//		if(sceneXML.tagName() == "Scene")
//			programName = sceneXML.attribute("name").toStdString();

		programName ="XML Loader";
		sceneLoader = new SceneLoader("media/scenes/test.xml");
	}



	void scene() {
		sceneLoader->load();

	}
};

int main(int argc, char *argv[]) {
	LoadApp().run();
}
