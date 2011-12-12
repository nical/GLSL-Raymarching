#include "SliderNodeView.hpp"

#include "kiwi/core/Node.hpp"
#include "kiwi/core/OutputPort.hpp"
#include "kiwi/core/NodeTypeManager.hpp"

#include <QSlider>
#include <QGraphicsScene>

#include "io/SliderNodeAdapter.hpp"

namespace io {

SliderNodeView::SliderNodeView(const QPointF& position, float smin, float smax)
: NodeView(position, kiwi::core::NodeTypeManager::Create("Float") )
{
    _min = smin;
    _max = smax;
    assert(node()->output(0).dataAs<float>());
    *node()->output(0).dataAs<float>() = (_min + _max) * 0.5;

    setWidth( 230 );
    setHeight( 70 );

    _slider = new QSlider(Qt::Horizontal);
    _slider->setMaximum(_max * 100);
    _slider->setMinimum(_min * 100);
    _slider->move(pos().x()+15, pos().y() + 40);

    _adapter = new SliderNodeAdapter(this);

    QObject::connect( _slider, SIGNAL(valueChanged(int)), _adapter, SLOT(setValue(int)));
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

    _slider->move(pos().x()+15, pos().y() + 40);
}

void SliderNodeView::updateValue(int val)
{
    *node()->output().dataAs<float>() = (float)val * 0.001;
}

}//namespace
