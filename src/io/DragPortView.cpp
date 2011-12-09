#include "DragPortView.hpp"

#include <QPainter>
#include <QColor>

namespace io{

DragPortView::DragPortView()
{
}


void DragPortView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if( isSelected() ){
        painter->setPen( QPen(QColor(50,50,100, 255),2) );
    }else{
        painter->setPen( QPen(QColor(100,100,100, 255),2) );
    }
    painter->setBrush( QColor(255,255,255, 255) );
    painter->drawEllipse( -5, -5, 10, 10 );
}

QRectF DragPortView::boundingRect() const
{
    return QRectF( -5, -5, 20, 20 );
}


}//namespace
