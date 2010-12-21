/*
 * RenderEngine.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include "Camera.h"
#include "MeshFactory.h"
#include "RenderEngine.h"
#include "Logger.h"

using namespace std;

RenderEngine::RenderEngine() : useFBO(false), frameCount(0), lightView(false), wire(false){
	glError("RenderEngine",23);
	checkVersion();
	shadowSequence = new RenderSequence();

	/* Make our background black */
	glClearColor(backgroundColor[0],backgroundColor[1],backgroundColor[2], 1.0);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_POINT_SPRITE_ARB);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	/** TODO: Deprecated*/
    //glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
    /*

     */
//    glEnable(GL_POLYGON_OFFSET_FILL);
    glPointSize(5);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glEnable(GL_BLEND);
    //glDepthMask(GL_FALSE);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glError("RenderEngine",52);

}

RenderEngine::~RenderEngine() {
	glError("RenderEngine",96);

    /* Cleanup all the things we bound and allocated */
    Logger::Instance().message << "Shutting down Render Engine...";
    Logger::Instance().log("MESSAGE");
	glError("RenderEngine",106);
}

void RenderEngine::toggleWire(){
	wire = !wire;
	if (wire) {
		Logger::Instance().log("MESSAGE", "Wireframe", "on");
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else{
		Logger::Instance().log("MESSAGE", "Wireframe", "off");
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void RenderEngine::toggleFBO(){
		if(useFBO){
		    Logger::Instance().message  << "FBO Rendering diabled";
		    Logger::Instance().log("MESSAGE");
			useFBO = false;
		}else{
			useFBO = true;
		    Logger::Instance().message << "FBO Rendering enabled";
		    Logger::Instance().log("MESSAGE");
		}
}

void RenderEngine::display() {


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


		shadowSequence->render();

}

void RenderEngine::toggleLightView(){
	if (lightView){
		lightView = false;
	}else{
		lightView = true;
	}
}

void RenderEngine::clear(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderEngine::checkVersion(){

	GLint maxTex1, maxTex2,MajorVersion,MinorVersion,numext,pointSize;

    Logger::Instance().message << glGetString(GL_VERSION);
    Logger::Instance().log("MESSAGE","OpenGL");

	glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
    Logger::Instance().message << MajorVersion<<"."<<MinorVersion;
    Logger::Instance().log("MESSAGE","Version");

    Logger::Instance().message << glGetString(GL_SHADING_LANGUAGE_VERSION);
    Logger::Instance().log("MESSAGE","GLSL");

	glGetIntegerv(GL_POINT_SIZE, &pointSize);
    Logger::Instance().message << pointSize;
    Logger::Instance().log("MESSAGE","Point Size");

    Logger::Instance().message << glGetString(GL_VENDOR) << " - " << glGetString(GL_RENDERER);
    Logger::Instance().log("MESSAGE","Hardware");

	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxTex1);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&maxTex2);
    Logger::Instance().message << maxTex1 << " " << maxTex2;
    Logger::Instance().log("MESSAGE","MaxTex");

	glGetIntegerv(GL_NUM_EXTENSIONS, &numext);
    Logger::Instance().message << "Found " << numext << " GL_EXTENSIONS";
    Logger::Instance().log("MESSAGE","GL_EXTENSIONS");

//	for (int i = 0; i < numext; i++) {
//		Logger::Instance().message << glGetStringi(GL_EXTENSIONS,i);
//		Logger::Instance().log("DEBUG","GL_EXTENSIONS");
//	}
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
