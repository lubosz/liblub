/*
 * RenderEngine.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include "Camera.h"
#include "MeshFactory.h"
#include "RenderEngine.h"

using namespace std;

RenderEngine::RenderEngine() {
	glError("RenderEngine",23);
	checkVersion();
	frameCount = 0;

#ifndef USE_GL3
    glEnable( GL_POINT_SMOOTH );
#endif

	/* Make our background black */
	glClearColor(backgroundColor[0],backgroundColor[1],backgroundColor[2], 1.0);

    glEnable(GL_POLYGON_OFFSET_FILL);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_POINT_SPRITE_ARB);

	/** TODO: Deprecated*/
	#ifndef USE_GL3
    //glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	#endif
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);

    /*
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0,2.0);
     */

    glPointSize(5);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glEnable(GL_BLEND);
    //glDepthMask(GL_FALSE);

#ifdef USE_FBO
    fbo = new FrameBuffer(1366,768);
#endif

	glError("RenderEngine",52);
}

RenderEngine::~RenderEngine() {
	glError("RenderEngine",96);
	delete fbo;
    /* Cleanup all the things we bound and allocated */
	cout << "Shutting down Render Engine...\n";
	glError("RenderEngine",106);
}

void RenderEngine::toggleFBO(){
	fbo->toggle();
}

void RenderEngine::display() {

#ifdef USE_FBO
		fbo->bind();
#endif

/*
 * 		//Uniform Animation
        SceneGraph::Instance().transform(frameCount);
        GLfloat floatanim = 10.0/GLfloat(frameCount%100);
        glUniform4f(glGetUniformLocation(shaderProgram->program, "ScaleFactor"), floatanim, floatanim, floatanim, floatanim);
		glUniform2f(glGetUniformLocation(shaderProgram->program, "Offset"), floatanim, floatanim);
		int mode = int(frameCount/100.0)%10;
		glUniform1i(glGetUniformLocation(shaderProgram->program, "Mode"), mode);
		cout << "Mode:\t" << mode << "\n";
		frameCount++;
*/
	clear();
    SceneGraph::Instance().drawNodes();

#ifdef USE_FBO
	fbo->draw();
#endif
}

void RenderEngine::clear(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderEngine::checkVersion(){

	GLint maxTex1, maxTex2,MajorVersion,MinorVersion,numext,pointSize;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxTex1);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&maxTex2);

	glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);

	glGetIntegerv(GL_POINT_SIZE, &pointSize);

	cout 	<< "OpenGL:\t" << glGetString(GL_VERSION) << "\n"
			<< "GLSL:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n"
			<< "Point Size:\t" << pointSize << "\n"
			//<< "GLU:\t" << gluGetString(GLU_VERSION) << "\n"
			<< "Hardware:\t" << glGetString(GL_VENDOR) << " - " << glGetString(GL_RENDERER) << "\n"
			//<< "GL_EXTENSIONS:\t" << glGetStringi(GL_EXTENSIONS,0) << "\n"
			<< "MaxTex:\t" << maxTex1 << " " << maxTex2 << "\n"
			<<"Version:\t"<<MajorVersion<<"."<<MinorVersion
			<< "\n";

#ifdef USE_GL3
	glGetIntegerv(GL_NUM_EXTENSIONS, &numext);
	cout << "Found " << numext << " GL_EXTENSIONS:\n";
	for (int i = 0; i < numext && DEBUG; i++){
		cout << glGetStringi(GL_EXTENSIONS,i) << " ";
	}
	cout << "\n";
#endif
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
