/*
 * MeshFactory.cpp
 *
 *  Created on: Jun 8, 2010
 *      Author: bmonkey
 */

#include "MeshFactory.h"
#include "Geometry.h"
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
	Mesh * mesh = load("earth.obj");
	mesh->setDrawType(GL_LINES);
	return mesh;
}

Mesh * MeshFactory::load(string file) {
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
			| aiProcess_JoinIdenticalVertices
			| aiProcess_SortByPType
			);

	// If the import failed, report it
	if (!scene) {
		cout << importer.GetErrorString() << "\n";
	}

	// Now we can access the file's contents.
	//DoTheSceneProcessing( scene);

	aiMesh * myAiMesh = scene->mMeshes[0];

	vector<GLfloat> positions;
	vector<GLfloat> normals;
	vector<GLfloat> tangents;
	vector<GLfloat> uvs;
	vector<GLuint> indices;

	unsigned numIndices = 0;

	for (unsigned i = 0; i < myAiMesh->mNumFaces; i++) {
		aiFace face = myAiMesh->mFaces[i];
		for (unsigned j = 0; j < face.mNumIndices; j++) {
			int vertex = face.mIndices[j];

			aiVector3D position = myAiMesh->mVertices[vertex];
			positions.push_back(position.x);
			positions.push_back(position.y);
			positions.push_back(position.z);

			aiVector3D normal = myAiMesh->mNormals[vertex];
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);

			aiVector3D tangent = myAiMesh->mTangents[vertex];
			tangents.push_back(tangent.x);
			tangents.push_back(tangent.y);
			tangents.push_back(tangent.z);

			aiVector3D uv = myAiMesh->mTextureCoords[0][vertex];
			uvs.push_back(uv.x);
			uvs.push_back(uv.y);

			indices.push_back(numIndices);
			numIndices++;

		}

	}
		/*
	for (unsigned i = 0; i < myAiMesh->mNumVertices; i++) {
		aiVector3D vertex = myAiMesh->mVertices[i];
		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);

		aiVector3D normal = myAiMesh->mNormals[i];
		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);

		aiVector3D uv = myAiMesh->mTextureCoords[0][i];
		uvs.push_back(uv.x);
		uvs.push_back(uv.y);

		indices.push_back(i);
	}
		*/


	Mesh * mesh = new Mesh();

	mesh->addBuffer(positions, 3, "in_Vertex");
	mesh->addBuffer(normals, 3, "in_Normal");
	mesh->addBuffer(tangents, 3, "in_Tangent");
	mesh->addBuffer(uvs, 2, "in_Uv");
	mesh->addElementBuffer(indices);
	mesh->setDrawType(GL_TRIANGLES);

	return mesh;

}
