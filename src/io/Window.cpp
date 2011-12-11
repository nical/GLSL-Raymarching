#include <GL/glew.h>

#include <stdio.h>

#include "io/Window.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/FrameBuffer.hpp"



#include <QApplication>
#include <QGLFormat>
#include <QTimer>

#include <iostream>

using namespace std;


namespace io{

  static const int TIMERMSECS = 10;

  int CurrentWidth;
  int CurrentHeight;
  int WindowHandle = 0;

  static renderer::Renderer* _renderer;

  GLWidget::GLWidget(const QGLFormat& format, QWidget *parent)
    : QGLWidget( /*format,*/ parent)
  {
    connect(&redrawClock, SIGNAL(timeout()), this, SLOT(update()));
    redrawClock.start(20);
  }

  GLWidget::~GLWidget(){

  }

  QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
  }

  QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
  }

  void GLWidget::initializeGL() {

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

  }


  void GLWidget::resizeGL(int Width, int Height)
  {

      cout << "resize\n";
    CurrentWidth = Width;
    CurrentHeight = Height;

    if (_renderer) {
      _renderer->setWindowDimensions(CurrentWidth, CurrentHeight);
    }
    renderer::ResizeFrameBuffers(Width,Height);
    glViewport(0, 0, CurrentWidth, CurrentHeight);

  }


  void GLWidget::paintGL()
  {

    //glutTimerFunc(millisec, RenderFunction, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // implement the scene rendering here.
    // like:
      _renderer->drawScene();
    //glutPostRedisplay();

  }

  void GLWidget::setRenderer (renderer::Renderer* r){
    std::cout << "Initialising renderer" << std::endl;
    r->init();
    r->createBuffers();
    _renderer = r;
  }

}//namespace
