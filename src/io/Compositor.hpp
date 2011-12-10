#ifndef COMPOSITOR_HPP
#define COMPOSITOR_HPP

#include <QGraphicsScene>

class QGraphicsView;

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

    bool tryConnect( PortView* p1, PortView * p2);
private:
    Compositor( QGraphicsView * v );

    QGraphicsView * _view;
    QGraphicsScene _scene;
};

}//namespace

#endif // COMPOSITOR_HPP

