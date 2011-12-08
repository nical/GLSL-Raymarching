#ifndef NODEVIEW_HPP
#define NODEVIEW_HPP

#include <QGraphicsItem>
#include <QRectF>
#include <QString>
#include <QGraphicsDropShadowEffect>

#include "kiwi/core/NodeTypeManager.hpp"
#include "kiwi/view/NodeView.hpp"

class QPointF;

namespace io{

class PortView;

class NodeView : public QGraphicsItem, public kiwi::view::NodeView
{
public:
    typedef std::vector<PortView*> PortArray;
    typedef const kiwi::core::NodeTypeInfo TypeInfo;

    explicit NodeView(const QPointF& position, kiwi::core::Node * n );

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mouseMoveEvent( QGraphicsSceneMouseEvent* event );

    TypeInfo * type();

    QRectF boundingRect() const
    {
        return _rect;
    }

    float leftX() const
    {
        return pos().x();
    }

    float rightX() const
    {
        return pos().x() + _rect.width();
    }

    float portsSpacing() const
    {
        return 15.0;
    }

    float inputsY() const
    {
        return pos().y() + headerHeight();
    }

    float headerHeight() const
    {
        return 30.0;
    }

    float outputsY() const
    {
        return pos().y() + headerHeight() + _inputs.size() * portsSpacing();
    }

    const PortArray& inputs() const
    {
        return _inputs;
    }
    const PortArray& outputs() const
    {
        return _outputs;
    }

    QPointF relativeInputPos(int i) const;
    QPointF relativeOutputPos(int i) const;

    void addToScene(QGraphicsScene * s);

    ~NodeView();
private:
    QGraphicsDropShadowEffect _dropShadow;
    QRectF _rect;
    PortArray _inputs;
    PortArray _outputs;
};


}//namespace

#endif // NODEVIEW_HPP
