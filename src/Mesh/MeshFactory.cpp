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
