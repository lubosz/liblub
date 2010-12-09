#include "Application.h"

class Example : public Application {

public:
	Example(){
		programName = "Minimal";
  }

	void scene(){
		SceneGraph::Instance().light = new Light(QVector3D(0.3, 6.6, -3.3),QVector3D(1,-5,-1));
/*
		SceneGraph::Instance().addNode(
	    		new Node(
	    				"Monkey",
	    				QVector3D(10, 0, -10),
	    				3.0,
	    				MeshFactory::load("monkey.blend"),
	    				new Simple("Color/white")
	    				//new PhongTexMat("Paper_Texture_by_Spiteful_Pie_Stock.jpg")
	    		)
	    );
	    */
	}

};

int main(int argc, char *argv[])
{
	Example().run();

}
