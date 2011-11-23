#pragma once
#ifndef IO_WINDOW_HPP
#define IO_WINDOW_HPP
#include <QApplication>
#include <QGLFormat>
#include <QTimer>

namespace renderer{
  class Renderer;
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

}//namespace


#endif
