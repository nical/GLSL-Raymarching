#pragma once
#ifndef IO_WINDOW_HPP
#define IO_WINDOW_HPP
#include <QApplication>
#include <QGLFormat>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QResizeEvent>

namespace renderer{
  class Renderer;
}

namespace io{


class GraphicsView : public QGraphicsView
{
public:
    GraphicsView()
    {
        setWindowTitle(tr("GLSL raymarcher"));
    }

protected:
    void resizeEvent(QResizeEvent *event) {
        if (scene())
            scene()->setSceneRect(
                QRect(QPoint(0, 0), event->size()));
        QGraphicsView::resizeEvent(event);
    }
};




class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene();
    void drawBackground(QPainter *painter, const QRectF &);
private:
    QDialog * createDialog( const QString &windowTitle) const;
};



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
