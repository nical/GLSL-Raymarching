#include "SliderNodeView.hpp"

#include "kiwi/core/Node.hpp"
#include "kiwi/core/OutputPort.hpp"
#include "kiwi/core/NodeTypeManager.hpp"

#include "io/PortView.hpp"
#include "io/Compositor.hpp"

#include <QSlider>
#include <QGraphicsScene>
#include <QPainter>

#include "io/SliderNodeAdapter.hpp"

namespace io {

void InsertSlider0_1( const QPointF& p)
{
    io::Compositor::Instance().add( new SliderNodeView(p, 0.0, 1.0) );
}

void InsertSlider0_10( const QPointF& p)
{
    io::Compositor::Instance().add( new SliderNodeView(p, 0.0, 10.0) );
}
void InsertSlider0_100( const QPointF& p)
{
    io::Compositor::Instance().add( new SliderNodeView(p, 0.0, 100.0) );
}

void AddSliderMenu()
{
    io::Compositor::Instance().addNodeToMenu("Slider [0;1]", &InsertSlider0_1 );
    io::Compositor::Instance().addNodeToMenu("Slider [0;10]", &InsertSlider0_10 );
    io::Compositor::Instance().addNodeToMenu("Slider [0;100]", &InsertSlider0_100 );
}

SliderNodeView::SliderNodeView(const QPointF& position, float smin, float smax)
: NodeView(position, kiwi::core::NodeTypeManager::Create("Float") )
{
    _min = smin;
    _max = smax;
    assert(node()->output(0).dataAs<float>());
    *node()->output(0).dataAs<float>() = (_min + _max) * 0.5;

    setWidth( 200 );
    setHeight( 80 );

    _slider = new QSlider(Qt::Horizontal);
    _slider->setMaximum(_max * 100);
    _slider->setMinimum(_min * 100);
    _slider->move(pos().x(), pos().y()+40);

    outputs()[0]->setPos( QPointF( rightX(), outputsY() ) );
    
    _adapter = new SliderNodeAdapter(this);

    setCacheMode(QGraphicsItem::NoCache);

    _slider->setValue((_min + _max) * 50.0);

    assert( QObject::connect( _slider, SIGNAL(valueChanged(int)), _adapter, SLOT(setValue(int))) );
}

void SliderNodeView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    NodeView::paint(painter, option, widget);
    QString valStr;
    valStr.setNum(*node()->output(0).dataAs<float>());
    painter->drawText( QRectF(5, 20, 50, 15), Qt::AlignCenter, valStr );

}

QString SliderNodeView::nodeName() const
{
    return "Slider";
}

void SliderNodeView::addToScene(QGraphicsScene * s)
{
    NodeView::addToScene(s);
    s->addWidget( _slider );
}

void SliderNodeView::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
    NodeView::mouseMoveEvent( event );

    _slider->move(pos().x(), pos().y() + 40);
}

void SliderNodeView::updateValue(int val)
{
    prepareGeometryChange();
    *node()->output().dataAs<float>() = (float)val * 0.01;
}

}//namespace
