#ifndef DRAGPORTVIEW_HPP
#define DRAGPORTVIEW_HPP

#include <QGraphicsItem>


namespace io{

class DragPortView : public QGraphicsItem
{
public:
    DragPortView();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
};


}//namespace

#endif // DRAGPORTVIEW_HPP
