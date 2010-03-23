/*
 * Mesh.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include "Mesh.h"
#include <iostream>

Mesh::Mesh(vector<GLfloat> position, vector<GLfloat> color, vector<GLfloat> normals, vector<GLubyte> index){
	bufferCount = 0;
	myASS = 0;

    /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &vao);

    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(vao);
    cout << "--buffer #" << bufferCount << "\n";
    cout << "--myASS #" << myASS << "\n";
    /* Allocate and assign three Vertex Buffer Objects to our handle */
    glGenBuffers(4, vbo);
    cout << "--buffer #" << bufferCount << "\n";
    cout << "--myASS #" << myASS << "\n";

	addBuffer(position);
	addBuffer(color);
	addBuffer(normals);

	addElementBuffer(index);
}

Mesh::~Mesh() {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDeleteBuffers(4, vbo);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::addBuffer(vector<GLfloat> content){
    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[myASS]);

	/* Copy the vertex data from tetrahedron to our buffer */
	/* 12 * sizeof(GLfloat) is the size of the tetrahedrom array, since it contains 12 GLfloat values */
	glBufferData(GL_ARRAY_BUFFER, content.size() * sizeof(GLfloat), content.data(), GL_STATIC_DRAW);

	/* Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex */
	glVertexAttribPointer((GLuint) myASS, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Enable attribute index 0 as being used */
	glEnableVertexAttribArray(myASS);
	myASS++;
	cout << "myASS #" << myASS << "\n";

}

void Mesh::addElementBuffer(vector<GLubyte> content){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[myASS]);
	/* Copy the index data from tetraindicies to our buffer
	 * 6 * sizeof(GLubyte) is the size of the index array, since it contains 6 GLbyte values */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, content.size() * sizeof(GLubyte), content.data(),GL_STATIC_DRAW);
	myASS++;
}
