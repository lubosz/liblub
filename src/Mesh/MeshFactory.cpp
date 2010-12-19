/*
 * MeshFactory.cpp
 *
 *  Created on: Jun 8, 2010
 *      Author: bmonkey
 */

#include "MeshFactory.h"
#include "Geometry.h"
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

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

Mesh *  MeshFactory::stars(float resolution, float density, float randomness,  float colorIntensity){
	vector<float> resolutionvec = {resolution,resolution,resolution};
	Mesh * mesh = Geometry::makeStars(resolutionvec,density,randomness, colorIntensity);
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
	Mesh * mesh = loadDirect("earth.obj");
	mesh->setDrawType(GL_LINES);
	return mesh;
}

Mesh * MeshFactory::load(string file) {
	return load(file, GL_TRIANGLES);
}

Mesh * MeshFactory::load(string file, GLint drawType) {
	string path = meshDir + file;

	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(path,
			aiProcess_CalcTangentSpace
			| aiProcess_Triangulate
			//| aiProcess_JoinIdenticalVertices
			//| aiProcess_SortByPType
			);

	// If the import failed, report it
	if (!scene) {
	    Logger::Instance().log("ERROR","Assimp Scene Load",importer.GetErrorString());
	}

	aiMesh * assMesh = scene->mMeshes[0];

	if(!assMesh->HasTangentsAndBitangents()){
		Logger::Instance().log("WARNING","Assimp Scene Load","NO TANGENTS!!!");
	}



	vector<GLfloat> positions;
	vector<GLfloat> normals;
	vector<GLfloat> tangents;
	vector<GLfloat> bitangents;
	vector<GLfloat> uvs;
	vector<GLuint> indices;

	unsigned numIndices = 0;

	for (unsigned i = 0; i < assMesh->mNumFaces; i++) {
		aiFace face = assMesh->mFaces[i];
		for (unsigned j = 0; j < face.mNumIndices; j++) {
			int vertex = face.mIndices[j];

			aiVector3D position = assMesh->mVertices[vertex];
			positions.push_back(position.x);
			positions.push_back(position.y);
			positions.push_back(position.z);

			aiVector3D normal = assMesh->mNormals[vertex];
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);

			if(assMesh->HasTangentsAndBitangents()){
				aiVector3D tangent = assMesh->mTangents[vertex];
				tangents.push_back(tangent.x);
				tangents.push_back(tangent.y);
				tangents.push_back(tangent.z);

				//assMesh->mBitangents

				aiVector3D bitangent = assMesh->mBitangents[vertex];

				bitangents.push_back(bitangent.x);
				bitangents.push_back(bitangent.y);
				bitangents.push_back(bitangent.z);
			}

			aiVector3D uv = assMesh->mTextureCoords[0][vertex];
			uvs.push_back(uv.x);
			uvs.push_back(uv.y);

			indices.push_back(numIndices);
			numIndices++;

		}

	}


	Mesh * mesh = new Mesh();

	mesh->addBuffer(positions, 3, "in_Vertex");
	mesh->addBuffer(normals, 3, "in_Normal");
	mesh->addBuffer(tangents, 3, "in_Tangent");
	mesh->addBuffer(bitangents, 3, "in_Bitangent");
	mesh->addBuffer(uvs, 2, "in_Uv");
	mesh->addElementBuffer(indices);
//	mesh->setDrawType(GL_TRIANGLES);
//	mesh->setDrawType(GL_PATCHES);
	mesh->setDrawType(drawType);

	return mesh;

}

Mesh * MeshFactory::loadDirect(string file) {
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
			aiProcess_SortByPType
			);

	// If the import failed, report it
	if (!scene) {
		Logger::Instance().log("ERROR","Assimp Scene Load",importer.GetErrorString());
	}

	// Now we can access the file's contents.
	//DoTheSceneProcessing( scene);

	aiMesh * assMesh = scene->mMeshes[0];

	vector<GLfloat> positions;
	vector<GLuint> indices;

	unsigned numIndices = 0;

	for (unsigned i = 0; i < assMesh->mNumVertices; i++) {
		aiVector3D vertex = assMesh->mVertices[i];
		positions.push_back(vertex.x);
		positions.push_back(vertex.y);
		positions.push_back(vertex.z);

		indices.push_back(i);
	}
	Mesh * mesh = new Mesh();
	mesh->addBuffer(positions, 3, "in_Vertex");
	mesh->addElementBuffer(indices);
	return mesh;

}
