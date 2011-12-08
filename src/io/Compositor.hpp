#ifndef COMPOSITOR_HPP
#define COMPOSITOR_HPP

#include <QGraphicsScene>

class QGraphicsView;

namespace io{


class Compositor
{
public:
    Compositor( QGraphicsView * v );
    ~Compositor();
private:
    QGraphicsView * _view;
    QGraphicsScene _scene;
};

}//namespace

#endif // COMPOSITOR_HPP

