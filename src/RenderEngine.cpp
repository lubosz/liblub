/*
 * RenderEngine.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */



#include "Camera.h"
#include "ObjLoader.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>

//#include <string>

#include "RenderEngine.h"
//#include "tnt.h"


using namespace std;

RenderEngine::RenderEngine() {
	glError("RenderEngine",23);
	checkVersion();
	frameCount = 0;

	shaderProgram = new ShaderProgram();
        sceneGraph = new SceneGraph();

	glError("RenderEngine",92);
}

RenderEngine::~RenderEngine() {
	glError("RenderEngine",96);
    /* Cleanup all the things we bound and allocated */
	cout << "Shutting down Render Engine...";

    delete shaderProgram;


	cout << "done.\n";
	glError("RenderEngine",106);
}



void RenderEngine::display() {
	//cout << frameCount << "\n";



        sceneGraph->transform(frameCount);

        sceneGraph->bindShaders(shaderProgram);

        //GLfloat floatanim = 10.0/GLfloat(frameCount%100);

	//glUniform4f(glGetUniformLocation(shaderProgram->program, "ScaleFactor"), floatanim, floatanim, floatanim, floatanim);
	//glUniform2f(glGetUniformLocation(shaderProgram->program, "Offset"), floatanim, floatanim);
	/*
	int mode = int(frameCount/100.0)%10;
	glUniform1i(glGetUniformLocation(shaderProgram->program, "Mode"), mode);
	cout << "Mode:\t" << mode << "\n";
*/
	/* Make our background black */
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Invoke glDrawElements telling it to draw a triangle strip using 6 indicies */
	//glDrawElements(GL_TRIANGLES, ObjLoader::Instance().mesh->indexSize, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
    //sceneGraph->translate(1,.2,.3);
    //sceneGraph->bindShaders(shaderProgram);
    //glDrawElements(GL_TRIANGLES, ObjLoader::Instance().mesh->indexSize, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);

	//glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_BYTE, 0);

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
	/*
	int * maxTex1 = new int();
	int * maxTex2 = new int();
*/
	GLint maxTex1, maxTex2,MajorVersion,MinorVersion,numext;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxTex1);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&maxTex2);

	glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);


	cout 	<< "OpenGL:\t" << glGetString(GL_VERSION) << "\n"
			<< "GLSL:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n"
			<< "GLU:\t" << gluGetString(GLU_VERSION) << "\n"
			<< "Hardware:\t" << glGetString(GL_VENDOR) << " - " << glGetString(GL_RENDERER) << "\n"
			//<< "GL_EXTENSIONS:\t" << glGetStringi(GL_EXTENSIONS,0) << "\n"
			<< "MaxTex:\t" << maxTex1 << " " << maxTex2 << "\n"
			//<< " " << glGetString(GL_MAX_TEXTURE_IMAGE_UNITS)
			<<"Version:\t"<<MajorVersion<<"."<<MinorVersion
			<< "\n";


	glGetIntegerv(GL_NUM_EXTENSIONS, &numext);
	cout << "Found " << numext << " GL_EXTENSIONS:\n";
	for (int i = 0; i < numext; i++){
		cout << glGetStringi(GL_EXTENSIONS,i) << " ";
	}
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
