#ifndef COMPOSITOR_HPP
#define COMPOSITOR_HPP

#include "io/Window.hpp"

class QGraphicsView;
class QAction;

namespace io{

class NodeView;
class PortView;

class Compositor
{
public:
    ~Compositor();

    static void Create( QGraphicsView * v );
    static Compositor& Instance();
    void add(NodeView * nv);
    QGraphicsScene * scene()
    {
        return &_scene;
    }

    void menu( QPoint pos );
    void addNodeToMenu( const QString& name, void(*fptr)(const QPointF&) );


    bool tryConnect( PortView* p1, PortView * p2);
private:
    Compositor( QGraphicsView * v );

    QGraphicsView * _view;
    GraphicsScene _scene;
};

}//namespace

#endif // COMPOSITOR_HPP

