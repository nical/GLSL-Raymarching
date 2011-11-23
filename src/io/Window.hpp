#pragma once
#ifndef IO_WINDOW_HPP
#define IO_WINDOW_HPP

namespace renderer{
  class Renderer;
}

namespace io{

  class GLWidget : public QGLWidget{
    Q_OBJECT;

    public:
      GLWidget(QWidget *parent = 0);
      ~GLWidget();

      QSize minimumSizeHint() const;
      QSize sizeHint() const;

    protected:
      void initializeGL();
      void paintGL();
      void resizeGL(int width, int height);
      void mousePressEvent(QMouseEvent *event);
      void mouseMoveEvent(QMouseEvent *event);
  }

  void InitWindow(renderer::Renderer* r, unsigned int w, unsigned int h, const char* name, int argc, char* argv[]);

}//namespace


#endif
