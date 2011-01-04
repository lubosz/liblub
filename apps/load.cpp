#include "Application.h"
#include "SceneLoader.h"

class LoadApp: public Application {

public:


	LoadApp(string sceneName) {

//		if(sceneXML.tagName() == "Scene")
//			programName = sceneXML.attribute("name").toStdString();

		programName ="XML Loader";
		QString sceneFile = QString::fromStdString(sceneName + ".xml");
		sceneLoader = new SceneLoader(sceneFile);
	}

	~LoadApp(){}

	void scene() {
		sceneLoader->load();

	}
};

int main(int argc, char *argv[]) {
	if (argc == 2){
		LoadApp(argv[1]).run();
	}else{
		Logger::Instance().log("NO SCENE SPECIFIED", "Try;", "./load test");
	}
}

