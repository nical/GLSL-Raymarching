#include "NodeView.hpp"

#include <QPointF>
#include <QPainter>
#include <QGraphicsScene>
#include <assert.h>

#include "kiwi/core/NodeTypeManager.hpp"
#include "kiwi/core/Node.hpp"
#include "kiwi/core/InputPort.hpp"
#include "kiwi/core/OutputPort.hpp"

#include "io/PortView.hpp"
#include "io/LinkView.hpp"

#include <iostream>

using namespace std;

namespace io{




NodeView::NodeView( const QPointF& position, kiwi::core::Node * n)
: kiwi::view::NodeView( n )
{
    assert( n );
    n->setView( this );
    setFlags(QGraphicsItem::ItemIsMovable);
    setPos( position );
    float nodeHeight = (nodeType()->inputs().size() + nodeType()->outputs().size()) * portsSpacing() + headerHeight();
    _rect = QRectF( 0, 0, 150.0, nodeHeight );

    int i = 0;
    for( auto it = nodeType()->inputs().begin(); it != nodeType()->inputs().end(); ++it )
    {
        _inputs.push_back( new PortView(PortView::INPUT, this, i) );
        _inputs[i]->setPos( QPointF( leftX(), inputsY() + i * portsSpacing() ) );
        ++i;
    }
    i = 0;
    for( auto it = nodeType()->outputs().begin(); it != nodeType()->outputs().end(); ++it )
    {
        _outputs.push_back( new PortView(PortView::OUTPUT, this, i) );
        _outputs[i]->setPos( QPointF( rightX(), outputsY() + i * portsSpacing() ) );
        ++i;
    }

    _dropShadow.setBlurRadius( 16 );
    _dropShadow.setXOffset( 0.0 );
    _dropShadow.setYOffset( 5.0 );
    setGraphicsEffect( &_dropShadow );

}

NodeView::~NodeView()
{
     if( scene() ) scene()->removeItem( this );
}

void NodeView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    assert(painter);

    QLinearGradient gradient( 0.0, 0.0, 0.0, _rect.bottom() );
    gradient.setColorAt( 1, Qt::gray );
    gradient.setColorAt( 0, Qt::white );
    painter->setBrush( gradient );
    painter->setPen( QPen( Qt::transparent ) );
    painter->drawRoundedRect( _rect, 10, 10 );

    painter->setPen( QPen( Qt::gray ) );
    painter->drawText( QRectF(0, 5, _rect.width(), 15), Qt::AlignCenter, nodeName() );
    painter->setPen( QPen( Qt::black ) );
    for(int i = 0; i < _inputs.size(); ++i )
    {
        float rip = relativeInputPos(i).y();
        painter->drawText( QRectF(10, rip-8.0, _rect.width(), rip+5.0), nodeType()->inputs()[i].name().c_str() );
    }

    for(int i = 0; i < _outputs.size(); ++i )
    {
        float rop = relativeOutputPos(i).y();
        painter->drawText( QRectF(10, rop-8.0f, _rect.width()-20.0, rop+5.0f), Qt::AlignRight, nodeType()->outputs()[i].name().c_str() );
    }
}

QString NodeView::nodeName() const
{
    return nodeType()->name().c_str();
}

const kiwi::core::NodeTypeInfo * NodeView::nodeType() const
{
    return node()->type();
}

QPointF NodeView::relativeInputPos(int i) const
{
    return QPointF( 0.0, headerHeight() + i * portsSpacing() );
}
QPointF NodeView::relativeOutputPos(int i) const
{
    return QPointF( _rect.width(), headerHeight() + _inputs.size() * portsSpacing() + i * portsSpacing() );
}

void NodeView::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
    QGraphicsItem::mouseMoveEvent( event );
    for(int i = 0; i < _inputs.size(); ++i)
    {
        if(_inputs[i]->state() != PortView::DRAG )
            _inputs[i]->setPos( QPointF( leftX(), inputsY() + i * portsSpacing() ) );
    }

    for(int i = 0; i < _outputs.size(); ++i)
    {
        // update port positions
        if(_outputs[i]->state() != PortView::DRAG )
            _outputs[i]->setPos( QPointF( rightX(), outputsY() + i * portsSpacing() ) );
        // update link positions
        for(int j = 0; j < _outputs[i]->connections().size(); ++j )
        {
            _outputs[i]->connections()[j]->updatePos();
        }
    }
}

void NodeView::addToScene(QGraphicsScene * s)
{
    assert(s);
    s->addItem(this);
    for( auto it = _inputs.begin(); it != _inputs.end(); ++it )
    {
        if(*it)
            s->addItem( *it );
    }
    for( auto it = _outputs.begin(); it != _outputs.end(); ++it )
    {
        if(*it)
            s->addItem( *it );
    }
}

int NodeView::indexOf( PortView* pv, int inputOrOutout ) const
{
    if( inputOrOutout != PortView::OUTPUT )
        for(int i = 0; i < _inputs.size(); ++i)
            if( _inputs[i] == pv )
                return i;
    if( inputOrOutout != PortView::INPUT)
        for(int i = 0; i < _outputs.size(); ++i)
            if( _outputs[i] == pv )
                return i;
    return -1;
}



void NodeView::outputConnected(kiwi::core::OutputPort* port, kiwi::core::InputPort* to)
{

}

void NodeView::inputConnected(kiwi::core::InputPort* port, kiwi::core::OutputPort* to)
{
    int in_i = port->index();
    int out_i = to->index();

    if( (in_i < 0) || (out_i < 0) )
    {
        std::cerr << "io::NodeView::outputConnected - error: negative index "<< out_i<<" "<< in_i<<"\n";
        return;
    }

    if( (!port->node()->view()) || (!to->node()->view()))
    {
        std::cerr << "io::NodeView::inputConnected - error: missing view\n";
        return;
    }

    auto io_nv = dynamic_cast<io::NodeView*>(to->node()->view());

    if( (in_i >= _inputs.size()) || (out_i >= io_nv->outputViews().size()) )
    {
        std::cerr << "io::NodeView::outputConnected - error: index too big "<< out_i<<" "<< in_i<<"\n";;
        return;
    }

    _inputs[in_i]->connect( io_nv->outputViews()[out_i] );
}

void NodeView::outputDisconnected(kiwi::core::OutputPort* port, kiwi::core::InputPort* from)
{

}

void NodeView::inputDisconnected(kiwi::core::InputPort* port, kiwi::core::OutputPort* from)
{
    std::cerr << "ioNodeView::inputDisconnected\n";
    int in_i = port->index();
    int out_i = from->index();

    if( (in_i < 0) || (out_i < 0) )
    {
        std::cerr << "io::NodeView::outputDisconnected - error: negative index "<< out_i<<" "<< in_i<<"\n";
        return;
    }

    if( (!port->node()->view()) || (!from->node()->view()))
    {
        std::cerr << "io::NodeView::inputDisconnected - error: missing view\n";
        return;
    }

    auto io_nv = dynamic_cast<io::NodeView*>(from->node()->view());

    if( (in_i >= _inputs.size()) || (out_i >= io_nv->outputViews().size()) )
    {
        std::cerr << "io::NodeView::outputDisconnected - error: index too big "<< out_i<<" "<< in_i<<"\n";;
        return;
    }
    _inputs[in_i]->disconnect();
}


}//namespace
