#include "ColorNodeView.hpp"

#include <QPainter>
#include "kiwi/core/Node.hpp"
#include "kiwi/core/OutputPort.hpp"
#include <QColor>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include "io/ColourPicker.hpp"
#include "glm/glm.hpp"

namespace io{

ColorNodeView::ColorNodeView(const QPointF& position, kiwi::core::Node * n )
: NodeView( position, n )
{
    colourPicker = new ColourPicker(n, this);
}

void ColorNodeView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    NodeView::paint(painter, option, widget);

    glm::vec3 * color = node()->output().dataAs<glm::vec3>();
    assert(colourPicker);


    painter->setBrush( QColor(color->r*255, color->g*255, color->b*255));
    //painter->setBrush( colourPicker->currentColor());
    painter->drawRect( QRectF( 10.0, 10.0, 25, 25.0 ) );
}

void ColorNodeView::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event ) {
    if (event->lastPos().x() >= 10.0 && event->lastPos().x() <= 35.0 && event->lastPos().y() >= 10.0 && event->lastPos().y() <= 35.0){
        colourPicker->open();
    }

}

void ColorNodeView::UpdateGraphics() {
    prepareGeometryChange();
}

QString ColorNodeView::nodeName() const
{
    return "Color";
}

}//nammespace
