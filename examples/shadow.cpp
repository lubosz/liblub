#include "common.h"
#include "Materials.h"
#include "Texture.h"
#include "Application.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Expressed as float so gluPerspective division returns a float and not 0 (640/480 != 640.0/480.0).

#define SHADOW_MAP_RATIO 2
#define RENDER_WIDTH 800
#define RENDER_HEIGHT 600

class ShadowExample : public Application {

public:
	//Light mouvement circle radius
	float light_mvnt;

	// Hold id of the framebuffer for light POV rendering
	GLuint fboId;

	// Use to activate/disable shadowShader
	GLhandleARB shadowShaderId;
	GLuint shadowMapUniform;

	Material * shadowMap;

	QMatrix4x4 projection, modelview;

	ShadowExample(){
		light_mvnt = 30.0f;
		width = 1920;
		height = 1200;
		programName = "ShadowExample";
  }

	void generateShadowFBO()
	{
		int shadowMapWidth = RENDER_WIDTH * SHADOW_MAP_RATIO;
		int shadowMapHeight = RENDER_HEIGHT * SHADOW_MAP_RATIO;

		GLenum FBOstatus;

		shadowMap = new ShadowMap(shadowMapWidth,shadowMapHeight);
		Texture * texture = shadowMap->textures[0];

		// create a framebuffer object
		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);

		// Instruct openGL that we won't bind a color texture with the currently binded FBO
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		// attach the texture to FBO depth attachment point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, texture->getHandler(), 0);

		// check FBO status
		FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
			printf("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO\n");

		// switch back to window-system-provided framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void setupMatrices(float position_x,float position_y,float position_z,float lookAt_x,float lookAt_y,float lookAt_z){

		projection = QMatrix4x4();
		projection.perspective(45,RENDER_WIDTH/RENDER_HEIGHT,10,40000);

		modelview = QMatrix4x4();
		modelview.lookAt({position_x,position_y,position_z},{lookAt_x,lookAt_y,lookAt_z},{0,1,0});
	}


	void setTextureMatrix(void){
		// This is matrix transform every coordinate x,y,z
		// x = x* 0.5 + 0.5
		// y = y* 0.5 + 0.5
		// z = z* 0.5 + 0.5
		// Moving from unit cube [-1,1] to [0,1]
		QMatrix4x4 bias = {
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
		};

		bias = projection * bias;
		bias = modelview * bias;
	}


	void renderScene(void)
	{

		//First step: Render from the light POV to a FBO, story depth values only
		glBindFramebuffer(GL_FRAMEBUFFER,fboId);	//Rendering offscreen

		//Using the fixed pipeline to render to the depthbuffer
		glUseProgram(0);

		// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
		glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_RATIO,RENDER_HEIGHT* SHADOW_MAP_RATIO);

		// Clear previous frame values
		glClear( GL_DEPTH_BUFFER_BIT);

		//Disable color rendering, we only want to write to the Z-Buffer
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);



		// Culling switching, rendering only backface, this is done to avoid self-shadowing
		glCullFace(GL_FRONT);
		SceneGraph::Instance().drawNodes();

		//Save modelview/projection matrice into texture7, also add a biais
		setTextureMatrix();


		// Now rendering from the camera POV, using the FBO to generate shadows
		glBindFramebuffer(GL_FRAMEBUFFER,0);

		glViewport(0,0,RENDER_WIDTH,RENDER_HEIGHT);

		//Enabling color write (previously disabled for light POV z-buffer rendering)
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		// Clear previous frame values
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Using the shadow shader
		shadowMap->activate();

		//setupMatrices(p_camera[0],p_camera[1],p_camera[2],l_camera[0],l_camera[1],l_camera[2]);
		//setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);

		glCullFace(GL_BACK);
		SceneGraph::Instance().drawNodes();

		// DEBUG only. this piece of code draw the depth buffer onscreen
/*
		 glUseProgram(0);
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 glOrtho(-RENDER_WIDTH/2,RENDER_WIDTH/2,-RENDER_HEIGHT/2,RENDER_HEIGHT/2,1,20);
		 glMatrixMode(GL_MODELVIEW);
		 glLoadIdentity();
		 glColor4f(1,1,1,1);
		 glActiveTextureARB(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D,depthTextureId);
		 glEnable(GL_TEXTURE_2D);
		 glTranslated(0,0,-1);
		 glBegin(GL_QUADS);
		 glTexCoord2d(0,0);glVertex3f(0,0,0);
		 glTexCoord2d(1,0);glVertex3f(RENDER_WIDTH/2,0,0);
		 glTexCoord2d(1,1);glVertex3f(RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
		 glTexCoord2d(0,1);glVertex3f(0,RENDER_HEIGHT/2,0);
*/
		 glDisable(GL_TEXTURE_2D);


		//glutSwapBuffers();
	}

	void scene(){
		Light * light = new Light(QVector4D(0.3, 6.6, -3.3, 1.0),QVector3D(1,-5,-1));
		SceneGraph::Instance().light = light;

		Mesh * ground = MeshFactory::Instance().load("plane.blend");
		Mesh * cube = MeshFactory::Instance().load("cube.blend");

		Material * groundColor = new PhongColorMat(QVector3D(0.3f,0.3f,0.3f));
		Material * cubeColor = new PhongColorMat(QVector3D(0.9f,0.9f,0.9f));

		SceneGraph::Instance().addNode(
				new Node(
						"Ground",
						{0,-3,0},
						10,
						ground,
						groundColor
				)
		);

		SceneGraph::Instance().addNode(
				new Node(
						"Cube1",
						{0,0,5},
						cube,
						groundColor
				)
		);

		SceneGraph::Instance().addNode(
				new Node(
						"Cube2",
						{0,0,-5},
						cube,
						groundColor
				)
		);


		//generateShadowFBO();

	}
};

int main(int argc, char *argv[])
{
	ShadowExample().run();

}
