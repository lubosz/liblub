/*
 * RenderEngine.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */



#include "Camera.h"
#include "MeshFactory.h"
#include "RenderEngine.h"

#define USE_FBO

using namespace std;

RenderEngine::RenderEngine() {
	glError("RenderEngine",23);
	checkVersion();
	frameCount = 0;

	//shaderProgram = new ShaderProgram();

	//glPointSize(3);
#ifndef USE_GL3
    glEnable( GL_POINT_SMOOTH );
#endif

    glEnable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_CULL_FACE);
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
    fbo = new FrameBuffer(800,800);

    renderPlane = MeshFactory::Instance().plane();
#endif
	glError("RenderEngine",52);
}

RenderEngine::~RenderEngine() {
	glError("RenderEngine",96);
    /* Cleanup all the things we bound and allocated */
	cout << "Shutting down Render Engine...";

    //delete shaderProgram;


	cout << "done.\n";
	glError("RenderEngine",106);
}



void RenderEngine::display() {

#ifdef USE_FBO
	fbo->bind();
#endif
    //GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
	//glDrawBuffers(2, buffers);


/*
        SceneGraph::Instance().transform(frameCount);

        SceneGraph::Instance().bindShaders(shaderProgram);
*/
        //GLfloat floatanim = 10.0/GLfloat(frameCount%100);

	//glUniform4f(glGetUniformLocation(shaderProgram->program, "ScaleFactor"), floatanim, floatanim, floatanim, floatanim);
	//glUniform2f(glGetUniformLocation(shaderProgram->program, "Offset"), floatanim, floatanim);
	/*
	int mode = int(frameCount/100.0)%10;
	glUniform1i(glGetUniformLocation(shaderProgram->program, "Mode"), mode);
	cout << "Mode:\t" << mode << "\n";
*/

	clear();
/*
    MeshFactory::Instance().meshes[0]->draw();
    SceneGraph::Instance().transform(frameCount);
    SceneGraph::Instance().translate(1,1,1);
    SceneGraph::Instance().bindShaders(shaderProgram);
    MeshFactory::Instance().meshes[1]->draw();
*/
    SceneGraph::Instance().drawNodes();

	//MeshFactory::Instance().drawMeshes();

	//gluLookAt(1, 0, 1, 1, 0, 0, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
	frameCount++;
#ifdef USE_FBO
	fbo->unBind();
	fbo->bindTexture();

	clear();

    renderPlane->draw();
#endif
}

void RenderEngine::clear(){
	/* Make our background black */
	glClearColor(backgroundColor[0],backgroundColor[1],backgroundColor[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderEngine::checkVersion(){
	/*
	int * maxTex1 = new int();
	int * maxTex2 = new int();
*/
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
			//<< " " << glGetString(GL_MAX_TEXTURE_IMAGE_UNITS)
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
