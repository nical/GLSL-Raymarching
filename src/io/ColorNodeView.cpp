#include "ColorNodeView.hpp"

#include <QPainter>
#include "kiwi/core/Node.hpp"
#include "kiwi/core/OutputPort.hpp"
#include <QColor>
#include <QRectF>
#include "glm/glm.hpp"

namespace io{

ColorNodeView::ColorNodeView(const QPointF& position, kiwi::core::Node * n )
: NodeView( position, n )
{
}

void ColorNodeView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    NodeView::paint(painter, option, widget);

    glm::vec3 * color = node()->output().dataAs<glm::vec3>();
    assert(color);

    painter->setBrush( QColor(color->r*255, color->g*255,color->b*255, 255 ) );
    painter->drawRect( QRectF( 10.0, 10.0, 25, 25.0 ) );
}

QString ColorNodeView::nodeName() const
{
    return "Color";
}

}//nammespace
