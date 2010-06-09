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
	objLoader = new ObjLoader();

}

MeshFactory::~MeshFactory() {
	// TODO Auto-generated destructor stub
}

void MeshFactory::tetrahedron(){
	meshes.push_back(Geometry::makeTetrahedron());
}

void MeshFactory::loadObj(string file){
	meshes.push_back(objLoader->readFile(file));
}

void MeshFactory::drawMeshes(){
    BOOST_FOREACH( Mesh* mesh, meshes )
    {
    	mesh->draw();
    }
}
