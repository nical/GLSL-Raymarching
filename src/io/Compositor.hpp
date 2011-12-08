#ifndef COMPOSITOR_HPP
#define COMPOSITOR_HPP

#include <QGraphicsScene>

class QGraphicsView;

namespace io{

class NodeView;

class Compositor
{
public:
    ~Compositor();

    static void Create( QGraphicsView * v );
    static Compositor& Instance();
    void add(NodeView * nv);
private:
    Compositor( QGraphicsView * v );

    QGraphicsView * _view;
    QGraphicsScene _scene;
};

}//namespace

#endif // COMPOSITOR_HPP

