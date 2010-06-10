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


}

MeshFactory::~MeshFactory() {
	// TODO Auto-generated destructor stub
}

void MeshFactory::tetrahedron(){
	meshes.push_back(Geometry::makeTetrahedron());
}

void MeshFactory::loadObj(string file){
	ObjLoader objLoader = ObjLoader(file);
	meshes.push_back(objLoader.mesh);
}

void MeshFactory::drawMeshes(){
    BOOST_FOREACH( Mesh* mesh, meshes )
    {
    	mesh->draw();
    }
}
