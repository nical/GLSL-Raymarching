#include "PortView.hpp"

#include <QPainter>
#include <QRectF>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <assert.h>
#include <iostream>

#include "io/LinkView.hpp"
#include "io/Compositor.hpp"

namespace io{


PortView::PortView( int iotype, NodeView* nv, int idx )
{
    setFlags(QGraphicsItem::ItemIsSelectable);
    _index = idx;
    _nodeView = nv;
    _alpha = 255;
    _io = iotype;
    _state = IDLE;
}

PortView::~PortView()
{
     if( scene() ) scene()->removeItem( this );
}

void PortView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if( isSelected() ){
        painter->setPen( QPen(QColor(200,100,30, _alpha),2) );
        painter->setBrush( QColor(255,255,255, _alpha) );
    }else{
        painter->setPen( QPen(QColor(0,0,0, _alpha),2) );
        painter->setBrush( QColor(5,255,50, _alpha) );
    }
    painter->drawEllipse( -5, -5, 10, 10 );

    _alpha = 255;
}

QRectF PortView::boundingRect() const
{
    return QRectF( -5, -5, 20, 20 );
}

bool PortView::isCompatible(PortView *p)
{
    return true;
}

bool PortView::connect(PortView *p)
{
    if( !isCompatible(p) ) return false;

    LinkView * link = 0;
    if(_io == OUTPUT)
        link = new LinkView(this,p);
    else
        link = new LinkView(p,this);

    _connections.push_back(link);
    p->_connections.push_back(link);

    scene()->addItem(link);
    link->setZValue( -100 );
}

bool PortView::disconnect()
{
    std::cerr << "io::PortView::disconnect()\n";
    assert( isInput() );

    assert( _connections.size() != 0  );
    if( _connections.size() == 0 )
        return false;

    // should happen once only!
    for( int i = 0; i < _connections.size(); ++i)
    {
        LinkView * l = _connections[i];
        PortView * op = l->outputView();

        // remove link from output port op
        for( int j = 0; j < op->_connections.size(); ++j )
        {
            if( op->_connections[j] == l )
            {
                int testOpSize = op->_connections.size();
                op->_connections[j] = op->_connections[ op->_connections.size()-1];
                op->_connections.resize(op->_connections.size()-1);
                assert( op->_connections.size() == testOpSize -1 );
            }
        }
        // remove link from this input port
        int testsize = _connections.size();
        _connections[i] = _connections[ _connections.size()-1];
        _connections.resize(_connections.size()-1);
        assert( _connections.size() == testsize-1 );

        // remove from scene
        l->scene()->removeItem( l );

        // delete without mercy
        delete l;
    }
    return true;

}

void PortView::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    bool deselect = false;

    auto s = scene()->selectedItems();

    std::cerr << "PortView::mousePressEvent\n";

    for(auto it = s.begin(); it != s.end(); ++it)
    {
        io::PortView * sp = dynamic_cast<io::PortView*>( *it );
        if( !sp )
            continue;
        if( Compositor::Instance().tryConnect( this, sp ) )
        {
            sp->setSelected(false);
            setSelected(false);
            deselect = true;
            break;
        }
        else
            std::cerr << "failed to connect\n";
    }

    QGraphicsItem::mousePressEvent(event);
    if(deselect) setSelected(false);
}

}//namespace
