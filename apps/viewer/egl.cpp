//#include <GL/gl.h>
#include <EGL/eglut.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "DemoLauncher.h"
#include "Renderer/OpenGL.h"
#include "Scene/Scene.h"

DemoLauncher * launcher;

static void draw(void) {
   launcher->draw();
   eglutPostRedisplay();
}

/* new window size or exposure */
static void reshape(GLint width, GLint height) {
//   GLfloat ar = static_cast<GLfloat> (width) / static_cast<GLfloat> (height);

   glViewport(0, 0, width, height);

//   glMatrixMode(GL_PROJECTION);
//   glLoadIdentity();
//   glFrustum(-ar, ar, -1, 1, 5.0, 60.0);

//   glMatrixMode(GL_MODELVIEW);
//   glLoadIdentity();
//   glTranslatef(0.0, 0.0, -10.0);
}

static void special_key(int special) {
   switch (special) {
   case EGLUT_KEY_LEFT:
       Scene::Instance().getCurrentCamera()->leftDirection(0.1);
      break;
   case EGLUT_KEY_RIGHT:
       Scene::Instance().getCurrentCamera()->rightDirection(0.1);
      break;
   case EGLUT_KEY_UP:
       Scene::Instance().getCurrentCamera()->forwardDirection(0.1);
      break;
   case EGLUT_KEY_DOWN:
       Scene::Instance().getCurrentCamera()->backwardDirection(0.1);
      break;
   default:
      break;
   }
}

int main(int argc, char *argv[]) {
   eglutInitWindowSize(1920, 1200);
   Scene::Instance().setResolution(1920, 1200);
   eglutInitAPIMask(EGLUT_OPENGL_BIT);
   eglutInit(argc, argv);

   eglutCreateWindow("liblub-egl");

   OpenGL::Instance().setContextCreated(true);

   launcher = new DemoLauncher(argc, argv);
   launcher->init();

   eglutReshapeFunc(reshape);
   eglutDisplayFunc(draw);
   eglutSpecialFunc(special_key);
   eglutMainLoop();

   return 0;
}
