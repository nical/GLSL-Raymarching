#pragma once
#ifndef IO_WINDOW_HPP
#define IO_WINDOW_HPP
#include <QApplication>
#include <QGraphicsScene>
#include <QGLWidget>
#include <QGLFormat>
#include <QTimer>
#include <QtGui>

namespace renderer{
  class Renderer;
  class FBOManager;
}

namespace io{

  class GLWidget : public QGLWidget{
    Q_OBJECT;

    public:
      GLWidget(const QGLFormat& format, QWidget *parent = 0);
      ~GLWidget();

      QSize minimumSizeHint() const;
      QSize sizeHint() const;

      void setRenderer (renderer::Renderer* r);

    protected:

      QTimer  redrawClock;

      void initializeGL();
      void paintGL();
      void resizeGL(int width, int height);
      //void mousePressEvent(QMouseEvent *event);
      //void mouseMoveEvent(QMouseEvent *event);
  };

  class GraphicsView : public QGraphicsView {

    public:
        GraphicsView();

    protected:
        void resizeEvent(QResizeEvent *event);
  };

  class OpenGLScene : public QGraphicsScene {
      Q_OBJECT

    public:
      OpenGLScene(renderer::Renderer* r, QGLWidget* blah);

      void drawBackground(QPainter *painter, const QRectF &rect);

    private:

      bool shaderInitialised;

      QGLWidget* hax;

      QTimer  redrawClock;
  };

}//namespace


#endif
