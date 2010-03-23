/*
 * RenderEngine.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include <stdio.h>
#include <string.h>

//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>

//#include <string>

#include "RenderEngine.h"

#include "utils.h"

using namespace std;

RenderEngine::RenderEngine() {
	glError("RenderEngine",23);
	checkVersion();
	frameCount = 0;

	tetrahedron = new Mesh();
	tetrahedron->makeTetrahedron();

    shaderProgram = new ShaderProgram();

    shaderProgram->attachShader("tutorial4.vert", GL_VERTEX_SHADER);
    shaderProgram->attachShader("tutorial4.geom", GL_GEOMETRY_SHADER);
    shaderProgram->attachShader("tutorial4.frag", GL_FRAGMENT_SHADER);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
    shaderProgram->bindAttrib("in_Position");
    shaderProgram->bindAttrib("in_Color");

    shaderProgram->linkAndUse();

    /* Create our projection matrix with a 45 degree field of view
     * a width to height ratio of 1 and view from .1 to 100 infront of us */
    perspective(projectionmatrix, 45.0, 1.0, 0.1, 100.0);
	glError("RenderEngine",92);
}

RenderEngine::~RenderEngine() {
	glError("RenderEngine",96);
    /* Cleanup all the things we bound and allocated */
	cout << "Shutting down Render Engine...";

    delete shaderProgram;
    delete tetrahedron;

	cout << "done.\n";
	glError("RenderEngine",106);
}

void RenderEngine::display() {
	//cout << frameCount << "\n";
	/* An identity matrix we use to perform the equivalant of glLoadIdentity */
	const GLfloat identitymatrix[16] = IDENTITY_MATRIX4;

	/* Load the identity matrix into modelmatrix. rotate the model, and move it back 5 */
	memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
	rotate(modelmatrix, (GLfloat) frameCount * -1.0, X_AXIS);
	rotate(modelmatrix, (GLfloat) frameCount * 1.0, Y_AXIS);
	rotate(modelmatrix, (GLfloat) frameCount * 0.5, Z_AXIS);
	translate(modelmatrix, 0, 0, -5.0);

	/* multiply our modelmatrix and our projectionmatrix. Results are stored in modelmatrix */
	multiply4x4(modelmatrix, projectionmatrix);

	/* Bind our modelmatrix variable to be a uniform called mvpmatrix in our shaderprogram */
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->program, "mvpmatrix"), 1,
			GL_FALSE, modelmatrix);

	/* Make our background black */
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Invoke glDrawElements telling it to draw a triangle strip using 6 indicies */
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, 0);

	frameCount++;
}

void RenderEngine::glError(string file, int line) {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}

		cerr << "GL_" << error << " - " << file << ':' << line	<< "\n";
		err = glGetError();
	}
}


void RenderEngine::checkVersion(){
	cout 	<< "OpenGL:\t" << glGetString(GL_VERSION) << "\n"
			<< "GLSL:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n"
			<< "Hardware:\t" << glGetString(GL_VENDOR) << " - " << glGetString(GL_RENDERER)
			//<< "GL_EXTENSIONS:\t" << glGetString(GL_EXTENSIONS) << "\n"
			<< "\n";
}

GLboolean RenderEngine::QueryExtension(char *extName) {
	char *p = (char *) glGetString(GL_EXTENSIONS);
	char *end;
	if (p == NULL)
		return GL_FALSE;
	end = p + strlen(p);
	while (p < end) {
		int n = strcspn(p, " ");
		if ((strlen(extName) == n) && (strncmp(extName, p, n) == 0)) {
			return GL_TRUE;
		}
		p += (n + 1);
	}
	return GL_FALSE;
}
