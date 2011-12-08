#include <GL/glew.h>

#include <stdio.h>

#include "io/Window.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/FrameBuffer.hpp"



#include <QApplication>
#include <QGLFormat>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QGraphicsItem>

#include <iostream>

using namespace std;


namespace io{

  static const int TIMERMSECS = 10;

  int CurrentWidth;
  int CurrentHeight;
  int WindowHandle = 0;

  static renderer::Renderer* _renderer;



QDialog * GraphicsScene::createDialog( const QString &windowTitle) const
{
    QDialog *dialog = new QDialog(0,
        Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    dialog->setWindowOpacity(0.8);
    dialog->setWindowTitle(windowTitle);
    dialog->setLayout(new QVBoxLayout);

    return dialog;
}

GraphicsScene::GraphicsScene()
{
     QWidget *instructions = createDialog(tr("Instructions"));
    instructions->layout()->addWidget(new QLabel(
        tr("Use mouse wheel to zoom model, and click and "
           "drag to rotate model")));
    instructions->layout()->addWidget(new QLabel(
        tr("Move the sun around to change the light "
           "position")));
    addWidget(instructions);

    QPointF pos(10, 10);
    foreach (QGraphicsItem *item, items()) {
        item->setFlag(QGraphicsItem::ItemIsMovable);
        item->setCacheMode(
            QGraphicsItem::DeviceCoordinateCache);
    
        const QRectF rect = item->boundingRect();
        item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
        pos += QPointF(0, 10 + rect.height());
    }
}


void GraphicsScene::drawBackground(QPainter *painter, const QRectF &)
{
    if (painter->paintEngine()->type() != QPaintEngine::OpenGL)
    {
        qWarning("OpenGLScene: drawBackground needs a "
                 "QGLWidget to be set as viewport on the "
                 "graphics view");
        //setViewport(new QGLWidget( QGLFormat(QGL::SampleBuffers)));
        //return;
    }

    glClearColor(1.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QTimer::singleShot(20, this, SLOT(update()));
}



  GLWidget::GLWidget(const QGLFormat& format, QWidget *parent)
    : QGLWidget( format, parent)
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
    return QSize(400, 400);  // TODO look at this ion case of size related pbs.
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
