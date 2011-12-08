#include "LinkView.hpp"
#include <QGraphicsScene>
#include <QPainter>

#include "io/PortView.hpp"

namespace io{

LinkView::LinkView(PortView* outputPort, PortView* inputPort)
{
    _inPort = inputPort;
    _outPort = outputPort;
}

LinkView::~LinkView()
{
     if( scene() ) scene()->removeItem( this );
}

void LinkView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath path( _outPort->pos() );
    path.cubicTo( (_inPort->pos().x()+_outPort->pos().x() )/2.0, _outPort->pos().y()
                 ,(_inPort->pos().x()+_outPort->pos().x() )/2.0, _inPort->pos().y()
                 , _inPort->pos().x(), _inPort->pos().y() );
    painter->setPen( QPen(Qt::blue, 3) );
    painter->drawPath( path );
    painter->drawRect( boundingRect() );
}

QRectF LinkView::boundingRect() const
{
    //return QRectF( QPointF(0.0,0.0), _inPort->pos() - _outPort->pos() );
    return QRectF( _outPort->pos(), _inPort->pos() );
}

void LinkView::updatePos()
{
    setPos( _outPort->pos() );
}

}//namespace
