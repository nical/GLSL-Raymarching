#ifndef PORTVIEW_HPP
#define PORTVIEW_HPP

#include <QGraphicsItem>

namespace io{

class NodeView;

class PortView : public QGraphicsItem
{
public:
    enum{ INPUT, OUTPUT };
    enum{ IDLE = 0, DRAG };

    PortView( int inputOrOutput, NodeView* nv, int idx );

    int index() const
    {
        return _index;
    }

    NodeView * nodeView() const
    {
        return _nodeView;
    }

    int state() const
    {
        return _state;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    NodeView * _nodeView;
    int _index;
    int _alpha;
    int _io;
    int _state;
};

}//namespace

#endif // PORTVIEW_HPP
