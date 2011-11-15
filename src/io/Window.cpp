
#include <stdio.h>

#include "io/Window.hpp"
#include "renderer/Renderer.hpp"


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace std;


namespace io{

    static const int TIMERMSECS = 10;

    int CurrentWidth;
    int CurrentHeight;
    int WindowHandle = 0;

  static renderer::Renderer* _renderer;

  void ResizeFunction(int Width, int Height)
  {
    
      cout << "resize\n";
    CurrentWidth = Width;
    CurrentHeight = Height;
    _renderer->setWindowDimensions(CurrentWidth, CurrentHeight);
    glViewport(0, 0, CurrentWidth, CurrentHeight);
    
  }


  void RenderFunction( int millisec )
  {
    
    glutTimerFunc(millisec, RenderFunction, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // implement the scene rendering here.
    // like:
    _renderer->drawScene();

    glutSwapBuffers();
    glutPostRedisplay();
    
  }

  void idleRendering(){
    
    glutPostRedisplay();
    
  }

  void InitWindow (renderer::Renderer* r, unsigned int w, unsigned int h, const char* name, int argc, char* argv[]){

    CurrentHeight = h;
    CurrentWidth = w;
    
    glutInit(&argc, argv);

    
    glutInitContextVersion(3, 3);
    //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
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
    glutTimerFunc(TIMERMSECS, RenderFunction, 0);
    //glutDisplayFunc(RenderFunction);

    
    GLenum GlewInitResult = glewInit();
    

    glViewport(0, 0, CurrentWidth, CurrentHeight);
    
    
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
    
    r->init();
    _renderer = r;
    
  }
}//namespace
