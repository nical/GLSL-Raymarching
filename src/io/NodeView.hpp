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

    virtual void outputConnected(kiwi::core::OutputPort* port, kiwi::core::InputPort* to);
    virtual void inputConnected(kiwi::core::InputPort* port, kiwi::core::OutputPort* to);
    virtual void outputDisconnected(kiwi::core::OutputPort* port, kiwi::core::InputPort* from);
    virtual void inputDisconnected(kiwi::core::InputPort* port, kiwi::core::OutputPort* from);


    QRectF boundingRect() const
    {
        return _rect;
    }

    const PortArray& inputs() const
    {
        return _inputs;
    }

    const PortArray& outputs() const
    {
        return _outputs;
    }

    TypeInfo * type();

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

    const PortArray& inputViews() const
    {
        return _inputs;
    }
    const PortArray& outputViews() const
    {
        return _outputs;
    }

    int indexOf( PortView* pv, int inputOrOutout = 0 ) const;

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
