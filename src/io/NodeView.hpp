#ifndef NODEVIEW_HPP
#define NODEVIEW_HPP

#include <QGraphicsItem>
#include <QRectF>
#include <QString>
#include <QGraphicsDropShadowEffect>

#include "kiwi/core/NodeTypeManager.hpp"

class QPointF;

namespace io{

class PortView;

class NodeView : public QGraphicsItem
{
public:
    typedef std::vector<PortView*> PortArray;
    typedef const kiwi::core::NodeTypeInfo TypeInfo;

    explicit NodeView(const QPointF& position, const kiwi::core::NodeTypeInfo* info );

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mouseMoveEvent( QGraphicsSceneMouseEvent* event );

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
        return pos().y() + 50.0;
    }

    float outputsY() const
    {
        return pos().y() + 50.0 + _inputs.size() * portsSpacing();
    }

    QPointF relativeInputPos(int i) const;
    QPointF relativeOutputPos(int i) const;

    void addToScene(QGraphicsScene * s);

    ~NodeView();
private:
    QGraphicsDropShadowEffect _dropShadow;
    QRectF _rect;
    TypeInfo * _info;
    PortArray _inputs;
    PortArray _outputs;
};


}//namespace

#endif // NODEVIEW_HPP
