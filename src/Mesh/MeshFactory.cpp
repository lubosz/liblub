/*
 * MeshFactory.cpp
 *
 *  Created on: Jun 8, 2010
 *      Author: bmonkey
 */

#include "MeshFactory.h"
#include "Geometry.h"
#include "ObjLoader.h"
#include <boost/foreach.hpp>
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

MeshFactory::MeshFactory() {
	/* available draw types */
	GL_POINTS,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_LINES,
#ifndef USE_GL3
	GL_POLYGON,
	GL_QUAD_STRIP,
	GL_QUADS,
#endif
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN,
	GL_TRIANGLES,
	GL_LINES_ADJACENCY,
	GL_LINE_STRIP_ADJACENCY,
	GL_TRIANGLES_ADJACENCY,
	GL_TRIANGLE_STRIP_ADJACENCY;

}

MeshFactory::~MeshFactory() {
	// TODO Auto-generated destructor stub
}

Mesh *  MeshFactory::tetrahedron(){
	Mesh * mesh = Geometry::makeTetrahedron();
	mesh->setDrawType(GL_TRIANGLE_STRIP);
	return mesh;
}

Mesh *  MeshFactory::cube(){
	Mesh * mesh = Geometry::makeCube();
	mesh->setDrawType(GL_TRIANGLES);
	return mesh;
}

Mesh *  MeshFactory::plane(){
	Mesh * mesh = Geometry::makePlane();
	mesh->setDrawType(GL_TRIANGLES);
	return mesh;
}

Mesh *  MeshFactory::stars(){
	vector<float> resolution = {100,100,100};
	Mesh * mesh = Geometry::makeStars(resolution,1.0,0.0);
	mesh->setDrawType(GL_POINTS);
	//meshes.push_back(Geometry::makeStars(resolution,0.4,2.9));
	return mesh;
}

Mesh *  MeshFactory::spiral(){
	Mesh * mesh = Geometry::makeSpiral(500000,1.0,0.0);
	mesh->setDrawType(GL_POINTS);
	return mesh;
}

Mesh *  MeshFactory::lamp(){
	ObjLoader objLoader = ObjLoader("earth.obj");
	objLoader.getMesh()->setDrawType(GL_LINES);
	return objLoader.getMesh();
}

Mesh * MeshFactory::load(string file){
	ObjLoader objLoader = ObjLoader(file);
	objLoader.getMesh()->setDrawType(GL_TRIANGLES);
	return objLoader.getMesh();
}

Mesh * MeshFactory::loadAssimp(string file) {
	string path = meshDir + file;
	/*/
	 const struct aiScene* scene = aiImportFile(path.c_str(),aiProcessPreset_TargetRealtime_Quality);
	 Importer myImporter * = new Importer();
	 */

	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(path,
			aiProcess_CalcTangentSpace
			| aiProcess_Triangulate
			//| aiProcess_JoinIdenticalVertices
			| aiProcess_SortByPType
			);

	// If the import failed, report it
	if (!scene) {
		cout << importer.GetErrorString() << "\n";
	}

	// Now we can access the file's contents.
	//DoTheSceneProcessing( scene);

	aiMesh * myAiMesh = scene->mMeshes[0];

	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLuint> indices;

	for (unsigned i = 0; i < myAiMesh->mNumVertices; i++) {
		aiVector3D vertex = myAiMesh->mVertices[i];
		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);

		aiVector3D normal = myAiMesh->mNormals[i];
		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);

		indices.push_back(i);
	}

	Mesh * mesh = new Mesh();

	mesh->addBuffer(vertices, 3, "in_Vertex");
	mesh->addBuffer(normals, 3, "in_Normal");
	mesh->addElementBuffer(indices);
	mesh->setDrawType(GL_TRIANGLES);

	return mesh;

}
