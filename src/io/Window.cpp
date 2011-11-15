
#include <stdio.h>

#include "io/Window.hpp"
#include "renderer/Renderer.hpp"


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace std;
#define CHECKERROR GLCheckError2(__LINE__,__PRETTY_FUNCTION__);

void GLCheckError2(int line, const char* comment = "")
  {
    GLuint err = glGetError();
    if( err != GL_NO_ERROR )
    {
        cout << "gl error at " <<line<<" "<< comment << ": " <<  err <<endl;
    }
  }



namespace io{

    static const int TIMERMSECS = 10;

    int CurrentWidth;
    int CurrentHeight;
    int WindowHandle = 0;

  static renderer::Renderer* _renderer;

  void ResizeFunction(int Width, int Height)
  {
    CHECKERROR
      cout << "resize\n";
    CurrentWidth = Width;
    CurrentHeight = Height;
    _renderer->setWindowDimensions(CurrentWidth, CurrentHeight);
    glViewport(0, 0, CurrentWidth, CurrentHeight);
    CHECKERROR
  }


  void RenderFunction( int millisec )
  {
    CHECKERROR
    glutTimerFunc(millisec, RenderFunction, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // implement the scene rendering here.
    // like:
    _renderer->drawScene();

    glutSwapBuffers();
    glutPostRedisplay();
    CHECKERROR
  }

  void idleRendering(){
    CHECKERROR
    glutPostRedisplay();
    CHECKERROR
  }

  void InitWindow (renderer::Renderer* r, unsigned int w, unsigned int h, const char* name, int argc, char* argv[]){

    CurrentHeight = h;
    CurrentWidth = w;
    
    glutInit(&argc, argv);

    CHECKERROR
    glutInitContextVersion(3, 3);
    //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    CHECKERROR
    glutSetOption(
      GLUT_ACTION_ON_WINDOW_CLOSE,
      GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

    CHECKERROR
    glutInitWindowSize(CurrentWidth, CurrentHeight);

    CHECKERROR
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    CHECKERROR
    WindowHandle = glutCreateWindow(name);

    if(WindowHandle < 1) {
      fprintf(
        stderr,
        "ERROR: Could not create a new rendering window.\n"
      );
      exit(EXIT_FAILURE);
    }

    CHECKERROR
    glutReshapeFunc(ResizeFunction);
    glutTimerFunc(TIMERMSECS, RenderFunction, 0);
    //glutDisplayFunc(RenderFunction);

    CHECKERROR
    GLenum GlewInitResult = glewInit();
    CHECKERROR

    glViewport(0, 0, CurrentWidth, CurrentHeight);
    CHECKERROR
    
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

    if(r==0)cout<<"r=0\n";
    CHECKERROR
    r->init();
    _renderer = r;
    CHECKERROR
  }
}//namespace
