#include "PortView.hpp"

#include <QPainter>
#include <QRectF>

namespace io{


PortView::PortView( int iotype, NodeView* nv, int idx )
{
    _index = idx;
    _nodeView = nv;
    _alpha = 255;
    _io = iotype;
    _state = IDLE;
}

void PortView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if( isSelected() ){
        painter->setPen( QPen(QColor(50,50,100, _alpha),2) );
    }else{
        painter->setPen( QPen(QColor(100,100,100, _alpha),2) );
    }
    painter->setBrush( QColor(255,255,255, _alpha) );
    painter->drawEllipse( -5, -5, 10, 10 );

    _alpha = 255;
}

QRectF PortView::boundingRect() const
{
    return QRectF( -5, -5, 20, 20 );
}

}//namespace
