
#include <stdio.h>

#include "io/Window.hpp"


#include <GL/glew.h>
#include <GL/freeglut.h>

namespace io{

  int CurrentWidth;
	int CurrentHeight;
	int WindowHandle = 0;

  static renderer::Renderer* _renderer;

  void ResizeFunction(int Width, int Height)
  {
    CurrentWidth = Width;
    CurrentHeight = Height;
    glViewport(0, 0, CurrentWidth, CurrentHeight);
  }

  void RenderFunction(void)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // implement the scene rendering here.
    // like:
    _render->drawScene();

    glutSwapBuffers();
    glutPostRedisplay();
  }

  void InitWindow (renderer::Renderer* r, unsigned int w, unsigned int h, const char* name, int argc, char* argv[]){

    CurrentHeight = h;
    CurrentWidth = w;

    glutInit(&argc, argv);

    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(
      GLUT_ACTION_ON_WINDOW_CLOSE,
      GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

    glutInitWindowSize(CurrentWidth, CurrentHeight);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    WindowHandle = glutCreateWindow(name);

    if(WindowHandle < 1) {
      fprintf(
        stderr,
        "ERROR: Could not create a new rendering window.\n"
      );
      exit(EXIT_FAILURE);
    }

    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);

    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
      fprintf(
        stderr,
        "ERROR: %s\n",
        glewGetErrorString(GlewInitResult)
      );
      exit(EXIT_FAILURE);
    }

    fprintf(
      stdout,
      "INFO: OpenGL Version: %s\n",
      glGetString(GL_VERSION)
    );

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    _renderer = r;
  }
}//namespace
