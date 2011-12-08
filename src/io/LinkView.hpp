#ifndef LINKVIEW_HPP
#define LINKVIEW_HPP

#include <QGraphicsItem>

namespace io{

class PortView;

class LinkView : public QGraphicsItem
{
public:
    LinkView( PortView* outputPort, PortView* inputPort );
    ~LinkView();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRectF boundingRect() const;

    void updatePos();

private:
    PortView * _outPort;
    PortView * _inPort;
};

}//namespace

#endif // LINKVIEW_HPP
