#include "Application.h"

class BufferTest : public Application {

public:
	BufferTest(){
		width = 1920;
		height = 1200;
		programName = "bufferTest";
  }

	void scene(){
		Material* material= new VertexColorMaterial();
		Material* material1= new NormalColorMat();
		Material* material2= new TextureMaterial("bunny.png");
		Mesh * plane = MeshFactory::Instance().plane();
		SceneGraph::Instance().addNode(plane,{-2,0,-5}, material);
		SceneGraph::Instance().addNode(plane,{0,0,-5}, material1);
		SceneGraph::Instance().addNode(plane,{2,0,-5}, material2);
	}
};

int main(int argc, char *argv[])
{
	BufferTest().run();

}
